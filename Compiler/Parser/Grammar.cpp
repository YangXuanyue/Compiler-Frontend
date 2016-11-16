#include "Grammar.h"
#include <iostream>

using std::cout;
using std::endl;

Grammar::Grammar() :
	//how to make this configurable?
	//what format should be adopted in the config file?
	nonterminals({"E", "T", "F"}),
	terminals({
		ADD, SUB, ASTERISK, DIV, L_PAREN, R_PAREN, NUMERIC_CONSTANT, END
	}),
	start_symbol("E"),
	productions({
		{"E", {"E", ADD, "T"}},
		{"E", {"E", SUB, "T"}},
		{"E", {"T"}},
		{"T", {"T", ASTERISK, "F"}},
		{"T", {"T", DIV, "F"}},
		{"T", {"F"}},
		{"F", {L_PAREN, "E", R_PAREN}},
		{"F", {NUMERIC_CONSTANT}}
	}) {
	int cur_idx = 0;
	for (const auto& production : productions) {
		production_idxes[production.left].insert(cur_idx++);
	}
	remove_left_recursion();
	//print_productions();
	extract_common_left_factor();
	print_productions();
	construct_first();
	//print_first();
	construct_follow();
	//print_follow();
}

void Grammar::print_productions() {
	for (const auto& nonterminal : nonterminals) {
		for (int i : production_idxes[nonterminal]) {
			cout << productions[i];	
		}
	}
	cout << endl;
}

void Grammar::print_first() {
	for (const auto& nonterminal : nonterminals) {
		cout << "first[" << nonterminal << "] = {";
		for (const auto& symbol : first[nonterminal]) {
			cout << token_vals[boost::get<TokenType>(symbol)] << ", ";
		}
		if (first[nonterminal].size()) {
			cout << "\b\b";
		}
		cout << "}\n";
	}
	cout << endl;
}

void Grammar::print_follow() {
	for (const auto& nonterminal : nonterminals) {
		cout << "follow[" << nonterminal << "] = {";
		for (const auto& symbol : follow[nonterminal]) {
			cout << token_vals[boost::get<TokenType>(symbol)] << ", ";
		}
		if (follow[nonterminal].size()) {
			cout << "\b\b";
		}
		cout << "}\n";
	}
	cout << endl;
}

Grammar::~Grammar() {
}

void Grammar::remove_left_recursion() {
	set<Symbol> vis_nonterminals;
	vector<Symbol> new_nonterminals;
	for (const auto& nonterminal : nonterminals) {
		set<int> new_production_idxes(production_idxes[nonterminal]);
		for (int i : production_idxes[nonterminal]) {
			const Symbol& first_symbol(productions[i].right.front());
			if (vis_nonterminals.find(first_symbol) != vis_nonterminals.end()) {
				new_production_idxes.erase(i);
				Production tmp_production(std::move(productions[i]));
				tmp_production.right.pop_front();
				for (int j : production_idxes[first_symbol]) {
					new_production_idxes.insert(productions.size());
					Production new_production(tmp_production);
					new_production.right.insert(
						new_production.right.begin(),
						productions[j].right.begin(),
						productions[j].right.end()
					);
				}
			}
		}
		production_idxes[nonterminal] = new_production_idxes;
		vector<int> left_recursive_production_idxes;
		for (int i : production_idxes[nonterminal]) {
			const Symbol& first_symbol(productions[i].right.front());
			if (first_symbol == nonterminal) {
				new_production_idxes.erase(i);
				left_recursive_production_idxes.push_back(i);
			}
		}
		if (left_recursive_production_idxes.size()) {
			production_idxes[nonterminal] = new_production_idxes;
			Symbol new_nonterminal(boost::get<string>(nonterminal) + "\'");
			new_nonterminals.emplace_back(new_nonterminal);
			for (int i : production_idxes[nonterminal]) {
				productions[i].right.push_back(new_nonterminal);
			}
			production_idxes[nonterminal] = std::move(new_production_idxes);
			production_idxes[new_nonterminal].insert(productions.size());
			productions.emplace_back(new_nonterminal, std::move(deque<Symbol>{EPSILON}));
			for (int i : left_recursive_production_idxes) {
				productions[i].left = new_nonterminal;
				productions[i].right.pop_front();
				productions[i].right.push_back(new_nonterminal);
				production_idxes[new_nonterminal].insert(i);
			}
		}
		vis_nonterminals.insert(nonterminal);
	}
	nonterminals.insert(
		nonterminals.end(),
		new_nonterminals.begin(),
		new_nonterminals.end()
	);
}

#include "../Trie.h"
#include <utility>
#include <boost/lexical_cast.hpp>

using std::pair;
using boost::lexical_cast;

enum {
	MAX_TRIE_SIZE = 500
};

void Grammar::extract_common_left_factor() {
	map<Symbol, int> symbol_to_idx;
	int cur_idx(0);
	for (const auto& nonterminal : nonterminals) {
		symbol_to_idx[nonterminal] = cur_idx++;
	}
	for (const auto& terminal : terminals) {
		symbol_to_idx[terminal] = cur_idx++;
	}
	Trie<int, -1> trie(MAX_TRIE_SIZE, cur_idx);
	vector<Symbol> new_nonterminals;
	//maps each symbol in each production to its idx
	//for insertion in trie
	vector<vector<int>> int_mapped_productions(productions.size());
	for (const auto& nonterminal : nonterminals) {
		trie.clear();
		for (int i : production_idxes[nonterminal]) {
			for (const auto& symbol : productions[i].right) {
				int_mapped_productions[i].push_back(symbol_to_idx[symbol]);
			}
			trie.insert<vector<int>, int>(int_mapped_productions[i], i);
		}
		set<int> remaining_production_idxes(production_idxes[nonterminal]);
		//clf = common left factor
		//in pair<vector<int>, int>:
		//vector<int> contanis indexes of productions shared clf
		//int indicates the length of their clf
		vector<pair<set<int>, int>> clf_shared_productions_sets;
		for (int i : production_idxes[nonterminal]) {
			if (remaining_production_idxes.find(i)
				!= remaining_production_idxes.end()) {
				int cur(trie.root);
				int clf_len(0);
				for (const auto& symbol : productions[i].right) {
					int idx(symbol_to_idx[symbol]);
					int nxt(trie.nodes[cur].next[idx]);
					if (trie.nodes[nxt].vis_vals.size() == 1) {
						if (trie.nodes[cur].vis_vals.size() > 1) {
							clf_shared_productions_sets.emplace_back(
								trie.nodes[cur].vis_vals, clf_len
							);
							for (int i : clf_shared_productions_sets.back().first) {
								remaining_production_idxes.erase(i);
								trie.erase<vector<int>, int>(int_mapped_productions[i], i);
							}
							break;
						}
					}
					cur = nxt;
					++clf_len;
				}
			}
		}
		if (clf_shared_productions_sets.size()) {
			production_idxes[nonterminal] = std::move(remaining_production_idxes);
			int new_nonterminal_suffix(1);
			for (auto& clf_shared_productions : clf_shared_productions_sets) {
				Symbol new_nonterminal(
					boost::get<string>(nonterminal)
					+ "_" + lexical_cast<string>(new_nonterminal_suffix++)
				);
				new_nonterminals.emplace_back(new_nonterminal);
				Production new_production(nonterminal);
				int clf_len(clf_shared_productions.second);
				auto clf_shared_production_idxes(
					std::move(clf_shared_productions.first)
				);
				bool has_init_new_production(false);
				for (int i : clf_shared_production_idxes) {
					productions[i].left = new_nonterminal;
					for (int j(0); j < clf_len; ++j) {
						if (!has_init_new_production) {
							new_production.right.emplace_front(
								std::move(productions[i].right.front())
							);
						}
						productions[i].right.pop_front();
					}
					has_init_new_production = true;
					if (productions[i].right.empty()) {
						productions[i].right.emplace_back(EPSILON);
					}
					production_idxes[new_nonterminal].insert(i);
				}
				production_idxes[nonterminal].insert(productions.size());
				productions.emplace_back(std::move(new_production));
			}
		}	
	}
}

void Grammar::construct_first() {
	first_of_production.resize(productions.size());
	for (const auto& nonterminal : nonterminals) {
		if (!has_constructed_first[nonterminal]) {
			construct_first(nonterminal);
		}
	}
}

void Grammar::construct_first(const Symbol& nonterminal) {
	for (int i : production_idxes[nonterminal]) {
		const auto& production(productions[i]);
		for (const auto& symbol : production.right) {
			if (symbol.which() == TERMINAL) {
				first_of_production[i].insert(symbol);
				break;
			}
			if (!has_constructed_first[symbol]) {
				construct_first(symbol);
			}
			first_of_production[i].insert(
				first[symbol].begin(),
				first[symbol].end()
			);
			if (first[symbol].find(EPSILON) == first[symbol].end()) {
				break;
			}
		}
		first[nonterminal].insert(
			first_of_production[i].begin(),
			first_of_production[i].end()
		);
	}
	has_constructed_first[nonterminal] = true;
}

void Grammar::construct_follow() {
	follow[start_symbol].insert(END);
	for (const auto& nonterminal : nonterminals) {
		if (!has_constructed_follow[nonterminal]) {
			construct_follow(nonterminal);
		}
	}
}

void Grammar::construct_follow(const Symbol& nonterminal) {
	for (const auto& tmp_nonterminal : nonterminals) {
		for (int i : production_idxes[tmp_nonterminal]) {
			const auto& production(productions[i]);
			for (int j(0), k; j < production.right.size(); ++j) {
				if (production.right[j] == nonterminal) {
					for (k = j + 1; k < production.right.size(); ++k) {
						const auto& symbol(production.right[k]);
						if (symbol.which() == TERMINAL) {
							follow[nonterminal].insert(symbol);
							break;
						}
						follow[nonterminal].insert(
							first[symbol].begin(),
							first[symbol].end()
						);
						if (first[symbol].find(EPSILON) == first[symbol].end()) {
							break;
						} else {
							follow[nonterminal].erase(EPSILON);
						}
					}
					if (k == production.right.size()
						&& production.left != nonterminal) {
						if (!has_constructed_follow[production.left]) {
							construct_follow(production.left);
						}
						follow[nonterminal].insert(
							follow[production.left].begin(),
							follow[production.left].end()
						);
					}
				}
			}
		}
	}
}

