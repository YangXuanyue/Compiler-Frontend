#include "LR_Parser.h"
#include <stack>
#include <queue>

using std::stack;
using std::deque;

namespace LR {
	ostream& out(cout);

	Parser& operator >> (const Lexer& lexer, Parser& parser) {
		const auto& token_stream(lexer.get_token_stream());
		if (token_stream.front().type == END) {
			return parser;
		}
		deque<pair<int, Symbol>> parsing_stack;
		parsing_stack.push_back({0, END});
		const auto& productions(grammar.get_productions());
		for (int i(0); i < token_stream.size(); ) {
			for (int j(0); j < parsing_stack.size(); ++j) {
				print_symbol(
					cout << "[" << parsing_stack[j].first << " ",
					parsing_stack[j].second
				) << "] ";
			}
			cout << endl;
			const Symbol& cur_symbol(token_stream[i].type);
			print_symbol(cout, cur_symbol) << endl;
			auto res(
				parser.action_table[
					parsing_stack.back().first
				].find(cur_symbol)
			);
			if (res != parser.action_table[parsing_stack.back().first].end()) {
				const auto& action(res->second);
				switch (action.type) {
					case SHIFT:
						parsing_stack.push_back({action.val, cur_symbol});
						++i;
						break;
					case REDUCE: {
						const auto& production(productions[action.val]);
						out << production;
						const auto& nonterminal(production.left);
						for (int j(production.right.size()); j--; parsing_stack.pop_back());
						int nxt(
							parser.goto_table[
								parsing_stack.back().first
							][
								nonterminal
							]
						);
						parsing_stack.push_back({nxt, nonterminal});
						break;
					}
					case ACC:
						return parser;
				}
			} else {
				out << "error\n";
				return parser;
			}
		}
		return parser;
	}

	void Parser::construct_parsing_table() {
		grammar.augment();
		//grammar.print_productions();
		grammar.construct_first();
		//grammar.print_first();
		grammar.construct_follow();
		//grammar.print_follow();
		action_table.resize(dfa.state_num);
		goto_table.resize(dfa.state_num);
		for (int cur(0); cur < dfa.state_num; ++cur) {
			for (const auto& transition : dfa.states[cur].next) {
				const Symbol& symbol(transition.first);
				int nxt(transition.second);
				if (symbol.which() == TERMINAL) {
					action_table[cur][symbol] = {SHIFT, nxt};
				} else {
					goto_table[cur][symbol] = nxt;
				}
			}
			for (const auto& reduced_item : dfa.item_sets[cur].reduced_items) {
				const auto& production(
					grammar.productions[
						reduced_item.production_idx
					]
				);
				const auto& nonterminal(production.left);
				if (nonterminal == grammar.start_symbol) {
					assert(grammar.follow[nonterminal].size() == 1
						   && *grammar.follow[nonterminal].begin() == END);
					action_table[cur][END] = ACC;
				} else {
					for (const auto& terminal : grammar.follow[nonterminal]) {
						if (action_table[cur].find(terminal) != action_table[cur].end()) {
							throw "Conflicted occured attempting \
                                   to construct SLR(1) parsing table.\n";
						} else {
							action_table[cur][terminal] = {REDUCE, reduced_item.production_idx};
						}
					}
				}
			}
		}
	}
}
	