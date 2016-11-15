#include "Parser.h"
#include <stack>
#include <iostream>
#include <utility>

using std::stack;
using std::cout;
using std::pair;

Parser& operator >>(const Lexer& lexer, Parser& parser) {
	const vector<Token>& token_stream(lexer.get_token_stream());
	stack<Symbol> parsing_stack;
	pair<deque<Symbol>, deque<Symbol>> left_sentencial_form{
		{}, {parser.grammar.get_start_symbol()}
	};
	parsing_stack.push(END);
	parsing_stack.push(parser.grammar.get_start_symbol());
	const vector<Production>& productions(parser.grammar.get_productions());
	cout << "parser outputs:\n";
	for (int i(0); i < token_stream.size(); ) {
		cout << "current left sentecial form:\n\t\t\t\t";
		for (const auto& symbol : left_sentencial_form.first) {
			print_symbol(cout, symbol);
			cout << " ";
		}
		for (const auto& symbol : left_sentencial_form.second) {
			print_symbol(cout, symbol);
			cout << " ";
		}
		cout << endl;
		cout << "current token stream:\n\t\t\t\t";
		for (int j(i); j < token_stream.size(); ++j) {
			print_symbol(cout, token_stream[j].type);
			cout << " ";
		}
		cout << endl;
		cout << "output:\n\t\t\t\t";
		const auto& token(token_stream[i]);
		if (parsing_stack.top().which() == TERMINAL) {
			if (boost::get<TokenType>(parsing_stack.top()) == token.type) {
				++i;
			} else {
				//handle error
				cout << "error: ";
				print_symbol(cout, token.type);
				cout << " expected\n";
			}
			parsing_stack.pop();
			if (left_sentencial_form.second.size()) {
				left_sentencial_form.first.push_back(
					left_sentencial_form.second.front()
				);
				left_sentencial_form.second.pop_front();
			}
		} else {
			auto res(parser.parsing_table[parsing_stack.top()].find(token.type));
			if (res != parser.parsing_table[parsing_stack.top()].end()) {
				parsing_stack.pop();
				left_sentencial_form.second.pop_front();
				if (res->second != Parser::SYNCH) {
					const Production& production(productions[res->second]);
					cout << production;
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
					cout << "error\n";
				}
			} else {
				//handle error
				++i;
				cout << "error\n";
			}
		}
		cout << endl;
	}
	cout << endl;
	return parser;
}

void Parser::construct_parsing_table() {
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
