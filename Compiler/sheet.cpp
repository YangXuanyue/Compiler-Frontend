#if 0
#pragma once

#include <boost/variant.hpp>
#include <string>
#include "TokenType.h"
#include <iostream>

using boost::variant;

using std::string;

using std::ostream;

enum SymbolType {
	NONTERMINAL, //string
	TERMINAL //TokenType
};

typedef variant<string, TokenType> Symbol;

void print_symbol(ostream& out, const Symbol& symbol);

#pragma once

#include <iostream>
#include <queue>
#include "Symbol.h"

using std::ostream;
using std::endl;

using std::deque;

struct Production {
	Symbol left;
	deque<Symbol> right;
};

#pragma once

#include <vector>
#include "Symbol.h"
#include "Production.h"
#include <map>
#include <set>
#include <iostream>

class Parser;

using std::vector;
using std::map;
using std::set;
using std::cout;
using std::endl;

	class Grammar {
	private:
		//非终结符集与终结符集
		vector<Symbol> nonterminals, terminals;
		//起始符号
		Symbol start_symbol;
		//产生式集
		vector<Production> productions;
		//每个非终结符为左端的产生式序号集
		map<Symbol, set<int>> production_idxes;
		//每个产生式右端文法符号串的FIRST集
		vector<set<Symbol>> first_of_production;
		//FIRST集与FOLLOW集
		map<Symbol, set<Symbol>> first, follow;
		//对每个非终结符是否已构造完FIRST集与FOLLOW集的标志，方便递归
		map<Symbol, bool> has_constructed_first, has_constructed_follow;
		//判断两个非终结符的FOLLOW集是否互相包含，避免求FOLLOW集过程出现无限递归
		map<Symbol, map<Symbol, bool>> includes_follow_of;

	public:
		Grammar();
		//从配置文件加载文法
		void load_from_ini();
		//消除左递归
		void remove_left_recursion();
		//提取左公因子
		void extract_common_left_factor();
		//对非终结符nonterminal构造FIRST集
		void construct_first(const Symbol& nonterminal);
		//对非终结符nonterminal构造FOLLOW集
		void construct_follow(const Symbol& nonterminal);
	};

void Grammar::remove_left_recursion() {
	set<Symbol> vis_nonterminals; //已对应产生式消除完左递归的非终结符集
	vector<Symbol> new_nonterminals; //新终结符集
	for (const auto& nonterminal : nonterminals) { //遍历非终结集
		set<int> new_production_idxes(production_idxes[nonterminal]);
		for (int i : production_idxes[nonterminal]) { //遍历非终结符nonterminal的所有产生式
			Symbol first_symbol(productions[i].right.front()); //产生式第一个符号
			//若firsr_symbol是一个已消除左递归的非终结符
			if (vis_nonterminals.find(first_symbol) != vis_nonterminals.end()) { 
				new_production_idxes.erase(i);
				//将该产生式的第一个符号分别替换为该符号的所有产生式
				productions[i].right.pop_front(); 
				for (int j : production_idxes[first_symbol]) {
					new_production_idxes.insert(productions.size());
					Production new_production(productions[i]);
					new_production.right.insert(
						new_production.right.begin(),
						productions[j].right.begin(),
						productions[j].right.end()
					);
					productions.emplace_back(std::move(new_production));
				}
			}
		}
		production_idxes[nonterminal] = new_production_idxes;
		vector<int> left_recursive_production_idxes; //含左递归的产生式序号集
		for (int i : production_idxes[nonterminal]) { //遍历非终结符nonterminal的产生式
			const Symbol& first_symbol(productions[i].right.front());
			//若出现了左递归，即第一个符号是nonterminal
			if (first_symbol == nonterminal) {
				//将该产生式从nonterminal的产生式集中删除
				new_production_idxes.erase(i);
				//再加入left_recursive_production_idxes中
				left_recursive_production_idxes.push_back(i);
			}
		}
		if (left_recursive_production_idxes.size()) { //若含有左递归
			production_idxes[nonterminal] = new_production_idxes;
			Symbol new_nonterminal(boost::get<string>(nonterminal) + "\'"); //新非终结符
			new_nonterminals.emplace_back(new_nonterminal);
			//消除nonterminal的直接左递归
			for (int i : production_idxes[nonterminal]) { 
				productions[i].right.push_back(new_nonterminal);
			}
			production_idxes[nonterminal] = std::move(new_production_idxes);
			production_idxes[new_nonterminal].insert(productions.size());
			productions.emplace_back(new_nonterminal, 
									 std::move(deque<Symbol>{EPSILON}));
			for (int i : left_recursive_production_idxes) {
				productions[i].left = new_nonterminal;
				productions[i].right.pop_front();
				productions[i].right.push_back(new_nonterminal);
				production_idxes[new_nonterminal].insert(i);
			}
		}
		vis_nonterminals.insert(nonterminal);
	}
	for (auto& new_nonterminal : new_nonterminals) {
		nonterminals.emplace_back(std::move(new_nonterminal));
	}
}

#include "Trie.h"
#include <utility>
#include <boost/lexical_cast.hpp>

using std::pair;
using boost::lexical_cast;

enum {
	MAX_TRIE_SIZE = 500
};

void Grammar::extract_common_left_factor() {
	//将每个符号映射到一个整数
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
		//对非终结符nonterminal的每个产生式，将其转化为整数映射值数组，插入到trie中
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
				//在该产生式对应trie树上路径中找第一个只有其经过的结点
				for (const auto& symbol : productions[i].right) {
					int idx(symbol_to_idx[symbol]);
					int nxt(trie.nodes[cur].next[idx]);
					if (trie.nodes[nxt].vis_vals.size() == 1) {
						if (trie.nodes[cur].vis_vals.size() > 1) {
							//trie根节点到其父结点路径长度即为该产生式集的左公因子长度
							clf_shared_productions_sets.emplace_back(
								trie.nodes[cur].vis_vals, clf_len
							);
							//将经过其父节点的产生式从trie中删除
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
				//对每一个含左公因子的产生式集都得增加一个新非终结符
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
				//提取左公因子
				for (int i : clf_shared_production_idxes) {
					productions[i].left = new_nonterminal;
					for (int j(0); j < clf_len; ++j) {
						if (!has_init_new_production) {
							new_production.right.emplace_back(
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
				new_production.right.emplace_back(std::move(new_nonterminal));
				productions.emplace_back(std::move(new_production));
			}
		}
	}
	for (auto& new_nonterminal : new_nonterminals) {
		nonterminals.emplace_back(std::move(new_nonterminal));
	}
}

//构造非终结符nonterminal的FIRST集
void Grammar::construct_first(const Symbol& nonterminal) {
	for (int i : production_idxes[nonterminal]) { //遍历nonterminal的产生式
		const auto& production(productions[i]);
		bool all_has_epsilon(true); //标记是否产生式所有符号的FIRST集都含epsilon
		for (const auto& symbol : production.right) { //遍历产生式的符号
			//若该符号是终结符，将其加入该产生式的FIRST集中并退出
			if (symbol.which() == TERMINAL) {
				first_of_production[i].insert(symbol);
				break;
			}
			//若还未构造该符号的FIRST集，递归构造该符号的FIRST集
			if (!has_constructed_first[symbol]) {
				construct_first(symbol);
			}
			//将该符号的FIRST集加入到该产生式的FIRST集中
			first_of_production[i].insert(
				first[symbol].begin(),
				first[symbol].end()
			);
			//若该符号的FIRST集中含epsilon，继续遍历，否则退出
			if (first[symbol].find(EPSILON) == first[symbol].end()) {
				all_has_epsilon = false;
				break;
			} else {
				//去掉epsilon
				first_of_production[i].erase(EPSILON);
			}
		}
		//若都含epsilon加入到，将epsilon加入到该产生式的FIRST集中
		if (all_has_epsilon) {
			first_of_production[i].insert(EPSILON);
		}
		//将该产生式的FIRST集加入到nonterminal的FIRST集中
		first[nonterminal].insert(
			first_of_production[i].begin(),
			first_of_production[i].end()
		);
	}
	//标记已构造nonterminal的FIRST集
	has_constructed_first[nonterminal] = true;
}

//构造非终结符nonterminal的FOLLOW集
void Grammar::construct_follow(const Symbol& nonterminal) {
	//遍历文法所有产生式
	for (const auto& tmp_nonterminal : nonterminals) { 
		for (int i : production_idxes[tmp_nonterminal]) {
			const auto& production(productions[i]);
			for (int j(0), k; j < production.right.size(); ++j) {
				if (production.right[j] == nonterminal) { //生成式中出现nonterminal
					for (k = j + 1; k < production.right.size(); ++k) {
						const auto& symbol(production.right[k]);
						if (symbol.which() == TERMINAL) { //终结符直接加入并退出
							follow[nonterminal].insert(symbol);
							break;
						}
						//依次加入后续符号的FIRST集除非该符号的FIRST集不含epsilon
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
					//若无后续符号或后续符号串的FIRST集都含epsilon
					if (k == production.right.size()
						&& production.left != nonterminal) {
						const auto& another_nonterminal(production.left);
						//标记nonterminal的FOLLOW集包含another_nonterminal的FOLLOW集
						includes_follow_of[nonterminal][another_nonterminal] = true;
						//若未标记another_nonterminal的FOLLOW集包含nonterminal的FOLLOW集
						if (!includes_follow_of[another_nonterminal][nonterminal]) {
							//若还未构造another_nonterminal的FOLLOW集，
							//递归构造another_nonterminal的FOLLOW集
							if (!has_constructed_follow[another_nonterminal]) {
								construct_follow(another_nonterminal);
							}
							//将another_nonterminal的FOLLOW集加入到nonterminal的FOLLOW集
							follow[nonterminal].insert(
								follow[another_nonterminal].begin(),
								follow[another_nonterminal].end()
							);
						}
					}
				}
			}
		}
	}
	//标记已构造nonterminal的FOLLOW集
	has_constructed_follow[nonterminal] = true;
}


void Grammar::construct_follow() {
	follow[start_symbol].insert(END);
	for (const auto& nonterminal : nonterminals) {
		if (!has_constructed_follow[nonterminal]) {
			construct_follow(nonterminal);
		}
	}
	//若有两个非终结符的FOLLOW集相互包含，将其并集赋给彼此
	for (const auto& nonterminal : nonterminals) {
		for (const auto& another_nonterminal : nonterminals) {
			if (includes_follow_of[nonterminal][another_nonterminal]
				&& includes_follow_of[another_nonterminal][nonterminal]) {
				follow[nonterminal].insert(
					follow[another_nonterminal].begin(),
					follow[another_nonterminal].end()
				);
				follow[another_nonterminal] = follow[nonterminal];
			}
		}
	}
}

#include "Lexer\Lexer.h"

class Parser {
	//重载流操作符让parser在lexer词法分析的结果上进行语法分析
	friend Parser& operator >> (const Lexer& lexer, Parser& parser);

private:
	Grammar grammar; //文法
	map<Symbol, map<Symbol, int>> parsing_table; //分析表

	void construct_parsing_table(); //构造分析表

public:
	enum {
		SYNCH = -1 //错误处理标志
	};

	Parser() {
		construct_parsing_table();
	}
};

void Parser::construct_parsing_table() {
	grammar.remove_left_recursion(); //消除左递归
	grammar.extract_common_left_factor(); //提取左公因子
	grammar.construct_first(); //构造FIRST集
	grammar.construct_follow(); //构造FOLLOW集
	for (const auto& nonterminal : grammar.nonterminals) { //遍历非终结符集
		for (int i : grammar.production_idxes[nonterminal]) { //遍历其产生式集
			bool has_epsilon = false; //标记是否为空产生式
			//对该产生式的FIRST集中的终结符，将该产生式填入对应表项中
			for (const auto& terminal : grammar.first_of_production[i]) {
				if (boost::get<TokenType>(terminal) != EPSILON) {
					parsing_table[nonterminal][terminal] = i;
				} else {
					has_epsilon = true;
				}
			}
			//若是空产生式
			if (has_epsilon) {
				//对该非终结符FOLLOW集中的终结符，将该产生式填入对应表项中
				for (const auto& terminal : grammar.follow[nonterminal]) {
					parsing_table[nonterminal][terminal] = i;
				}
			}
		}
		//对于该非终结符FOLLOW集中的终结符，若对应表项为空，填入用于错误处理的同步信息SYNCH
		for (const auto& terminal : grammar.follow[nonterminal]) {
			if (parsing_table[nonterminal].find(terminal)
				== parsing_table[nonterminal].end()) {
				parsing_table[nonterminal][terminal] = SYNCH;
			}
		}
	}
}

#include "Parser.h"
#include <stack>
#include <iostream>
#include <fstream>
#include <utility>

using std::stack;
using std::ofstream;
using std::cout;
using std::pair;

ofstream out("../Compiler/Parser/parser_output.txt");

Parser& operator >> (const Lexer& lexer, Parser& parser) {
	//经词法分析得到的token流
	const vector<Token>& token_stream(lexer.get_token_stream());
	//分析栈
	stack<Symbol> parsing_stack;
	//左句型
	pair<deque<Symbol>, deque<Symbol>> left_sentencial_form{
		{},{parser.grammar.get_start_symbol()}
	};
	//将结尾符号与文法起始符号压入分析栈
	parsing_stack.push(END);
	parsing_stack.push(parser.grammar.get_start_symbol());
	const vector<Production>& productions(parser.grammar.get_productions());
	//扫描token流
	for (int i(0); i < token_stream.size(); ) {
		out << "current left sentencial form:\n\t\t\t\t";
		for (const auto& symbol : left_sentencial_form.first) {
			print_symbol(out, symbol);
			out << " ";
		}
		for (const auto& symbol : left_sentencial_form.second) {
			print_symbol(out, symbol);
			out << " ";
		}
		out << endl;
		out << "current token stream:\n\t\t\t\t";
		for (int j(i); j < token_stream.size(); ++j) {
			print_symbol(out, token_stream[j].type);
			out << " ";
		}
		out << endl;
		out << "output:\n\t\t\t\t";
		const auto& token(token_stream[i]);
		//若当前分析栈顶为终结符
		if (parsing_stack.top().which() == TERMINAL) {
			//若该终结符与待输入token流第一个token匹配
			if (boost::get<TokenType>(parsing_stack.top()) == token.type) {
				//指针移进
				++i;
			} else {
				//否则出现错误，给出错误信息（后续会弹栈）
				out << "error: ";
				print_symbol(out, token.type);
				out << " expected\n";
			}
			//弹栈
			parsing_stack.pop();
			//更新当前左句型
			if (left_sentencial_form.second.size()) {
				left_sentencial_form.first.push_back(
					left_sentencial_form.second.front()
				);
				left_sentencial_form.second.pop_front();
			}
		} else {
			//栈顶为非终结符，在分析表中查找对应动作
			auto res(parser.parsing_table[parsing_stack.top()].find(token.type));
			if (res != parser.parsing_table[parsing_stack.top()].end()) {
				//若表项非空白，弹栈
				parsing_stack.pop();
				left_sentencial_form.second.pop_front();
				if (res->second != Parser::SYNCH) {
					//若表项不是同步信息SYNCH，
					//将使用对应产生式压栈，并更新当前左句型
					const Production& production(productions[res->second]);
					out << production;
					if (!(production.right.front().which() == TERMINAL
						  && boost::get<TokenType>(production.right.front())
						     == EPSILON)) {
						for (int j(production.right.size() - 1); ~j; --j) {
							parsing_stack.push(production.right[j]);
						}
						left_sentencial_form.second.insert(
							left_sentencial_form.second.begin(),
							production.right.begin(),
							production.right.end()
						);
					}
				} else {
					//若为SYNCH给出错误提示（已弹栈）
					out << "error\n";
				}
			} else {
				//若表项为空白，将指针移进并给出错误提示
				++i;
				out << "error\n";
			}
		}
		out << endl;
	}
	out << endl;
	return parser;
}

#endif // 1
