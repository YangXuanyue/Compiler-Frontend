#include "LR_Parser.h"
#include <stack>
#include <queue>
#include <tuple>
#include <boost/variant.hpp>

using std::stack;
using std::deque;
using std::tuple;
using boost::variant;

namespace LR {
	typedef tuple<int, Symbol, variant<long long, double>, TokenType> LrStackItem;

	enum {
		STATE_IDX,
		SYMBOL,
		VAL,
		TYPE
	};

	ostream& print_val(ostream& out, variant<long long, double> val, TokenType type) {
		switch (type) {
			case INT_CONSTANT:
				out << boost::get<long long>(val);
				break;
			case REAL_CONSTANT:
				out << boost::get<double>(val);
				break;
		}
		return out;
	}

	Parser& operator >>(const Lexer& lexer, Parser& parser) {
		ostream& out(cout);
		const auto& token_stream(lexer.get_token_stream());
		if (token_stream.front().type == END) {
			return parser;
		}
		deque<LrStackItem> parsing_stack;
		//将I0入栈
		parsing_stack.push_back({0, END, 0., END});
		const auto& productions(grammar.get_productions());
		for (int i(0); i < token_stream.size(); ) {
			out << endl;
			out << "current parsing stack:\n\t\t\t\t";
			for (int j(0); j < parsing_stack.size(); ++j) {
				print_symbol(
					out << "[" << std::get<STATE_IDX>(parsing_stack[j]) << " ",
					std::get<SYMBOL>(parsing_stack[j])
				) << "] ";
			}
			out << endl;
			const Token& cur_token(token_stream[i]);
			const Symbol& cur_symbol(cur_token.type);
			out << "current token stream:\n\t\t\t\t";
			for (int j(i); j < token_stream.size(); ++j) {
				print_symbol(out, token_stream[j].type) << " ";
			}
			out << endl;
			out << "output:\n\t\t\t\t";
			auto res(
				parser.action_table[
					std::get<STATE_IDX>(parsing_stack.back())
				].find(cur_symbol)
			);
			if (res != parser.action_table[std::get<STATE_IDX>(parsing_stack.back())].end()) {
				const auto& action(res->second);
				out << action;
				switch (action.type) {
					case SHIFT: {
						parsing_stack.push_back({action.val, cur_symbol, cur_token.val, cur_token.type});
						++i;
						break;
					}
					case REDUCE: { //归约
						const auto& production(productions[action.val]);
						const auto& nonterminal(production.left);
						int new_top_pos(parsing_stack.size() - production.right.size());
						out << "current subexpression:\n\t\t\t\t";
						for (int j(new_top_pos); j < parsing_stack.size(); ++j) {
							if (is_int_or_real(std::get<TYPE>(parsing_stack[j]))) {
								print_val(
									out,
									std::get<VAL>(parsing_stack[j]),
									std::get<TYPE>(parsing_stack[j])
								) << " ";
							} else {
								out << token_vals[std::get<TYPE>(parsing_stack[j])] << " ";
							}
						}
						out << "\n\t\t\t\t";
						int nxt(
							parser.goto_table[
								std::get<STATE_IDX>(
									parsing_stack[
										new_top_pos - 1
									]
								)
							][nonterminal]
						);
						auto& new_top = parsing_stack[new_top_pos];
						std::get<STATE_IDX>(new_top) = nxt;
						std::get<Symbol>(new_top) = nonterminal;
						production.translate(parsing_stack);
						for (int j(production.right.size() - 1); j--; parsing_stack.pop_back());
						print_val(
							out << "type = "
								<< token_type_strs[std::get<TYPE>(new_top)] 
								<< "\n\t\t\t\tvalue = ",
							std::get<VAL>(new_top),
							std::get<TYPE>(new_top)
						) << endl;
						break;
					}
					case ACC: //接受
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
	