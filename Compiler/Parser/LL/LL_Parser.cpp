#include "LL_Parser.h"
#include <stack>
#include <iostream>
#include <fstream>
#include <utility>

using std::stack;
using std::ofstream;
using std::cout;
using std::pair;

namespace LL {
	//ofstream out("../Compiler/Parser/parser_output.txt");
	ostream& out(cout);

	Parser& operator >> (const Lexer& lexer, Parser& parser) {
		const auto& token_stream(lexer.get_token_stream());
		if (token_stream.front().type == END) {
			return parser;
		}
		stack<Symbol> parsing_stack;
		parsing_stack.push(END);
		parsing_stack.push(grammar.get_start_symbol());
		pair<deque<Symbol>, deque<Symbol>> left_sentencial_form{
			{},{grammar.get_start_symbol()}
		};
		const auto& productions(grammar.get_productions());
		//cout << "parser outputs:\n";
		for (int i(0); i < token_stream.size() && parsing_stack.size(); ) {
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
			const auto& cur_symbol(token_stream[i].type);
			if (parsing_stack.top().which() == TERMINAL) {
				if (boost::get<TokenType>(parsing_stack.top()) == cur_symbol) {
					++i;
				} else {
					//handle error
					out << "error: ";
					print_symbol(out, parsing_stack.top());
					out << " expected\n";
				}
				parsing_stack.pop();
				if (left_sentencial_form.second.size()) {
					left_sentencial_form.first.push_back(
						left_sentencial_form.second.front()
					);
					left_sentencial_form.second.pop_front();
				}
			} else {
				auto res(
					parser.parsing_table[
						parsing_stack.top()
					].find(cur_symbol)
				);
				if (res != parser.parsing_table[parsing_stack.top()].end()) {
					Symbol nonterminal(std::move(parsing_stack.top()));
					parsing_stack.pop();
					left_sentencial_form.second.pop_front();
					if (res->second != Parser::SYNCH) {
						const Production& production(productions[res->second]);
						out << production;
						if (!(production.right.front().which() == TERMINAL
							  && boost::get<TokenType>(production.right.front()) == EPSILON)) {
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
						//handle error
						out << "error: " << nonterminal << " expected\n";
					}
				} else {
					//handle error
					out << "error: ";
					print_symbol(out, Symbol(token_stream[i].type));
					out << " unexpected\n";
					++i;
				}
			}
			out << endl;
		}
		out << endl;
		return parser;
	}

	void Parser::construct_parsing_table() {
		grammar.remove_epsilon_production();
		grammar.remove_left_recursion();
		grammar.print_productions();
		grammar.extract_common_left_factor();
		//grammar.print_productions();
		grammar.construct_first();
		//grammar.print_first();
		grammar.construct_follow();
		//grammar.print_follow();
		for (const auto& nonterminal : grammar.nonterminals) {
			for (int i : grammar.production_idxes[nonterminal]) {
				bool has_epsilon = false;
				for (const auto& terminal : grammar.first_of_production[i]) {
					if (boost::get<TokenType>(terminal) != EPSILON) {
						parsing_table[nonterminal][terminal] = i;
					} else {
						has_epsilon = true;
					}
				}
				if (has_epsilon) {
					for (const auto& terminal : grammar.follow[nonterminal]) {
						parsing_table[nonterminal][terminal] = i;
					}
				}
			}
			for (const auto& terminal : grammar.follow[nonterminal]) {
				if (parsing_table[nonterminal].find(terminal)
					== parsing_table[nonterminal].end()) {
					parsing_table[nonterminal][terminal] = SYNCH;
				}
			}
		}
	}
}
