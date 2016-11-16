#include "Lexer.h"
#include <iostream>
#include "../SymbolTable.h"
#include "../Trie.h"

inline bool is_blank_char(char c) {
	return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\0');
}

DFA* Lexer::dfa = nullptr;

Lexer::Lexer(char _end_char) :
	end_char(_end_char) {
	if (dfa == nullptr) {
		dfa = new DFA;
	}
}

const Lexer& operator >>(istream& in, Lexer& lexer) {
	lexer.token_stream.clear();
	symbol_table.clear();
	lexer.dfa->init();
	lexer.char_cnt = 0;
	fill(lexer.token_type_cnts.begin(), lexer.token_type_cnts.end(), 0);
	lexer.error_token_ids.clear();

	Token cur_token;
	string cur_symbol;
	Trie<int, Lexer::NOT_FOUND> idx_in_symbol_table(Lexer::MAX_TRIE_SIZE);
	int cur_row(1), cur_col(1);
	bool reaches_end = false; 
	bool expects_new_token = true; 
	for (char c; !reaches_end; ) {
		if ((c = in.get()) == lexer.end_char) {
			//for recognition of token just before EOF
			//since '\0' does not exist in the C-code text, 
			//use it to substitute EOF = -1
			c = '\0'; 
			reaches_end = true;
		}
		if (expects_new_token && !is_blank_char(c)) {
			cur_token.row = cur_row; 
			cur_token.col = cur_col;
			expects_new_token = false; 
		}
		Lexer::dfa->trans(c); 
		if (Lexer::dfa->needs_retract()) { 
			in.putback(c); 
		} else {
			if (Lexer::dfa->get_state() != DFA::START) {
				cur_symbol += c; 
			} else if (!is_blank_char(c)) {
				cur_symbol += c; 
			}
			++lexer.char_cnt; 
			++cur_col; 
			if (c == '\n') {
				++cur_row; 
				cur_col = 1;
			}
		}
		if (Lexer::dfa->get_output() != UNKNOWN) {
			cur_token.type = Lexer::dfa->get_output();		
			if (cur_token.type == IDENTIFIER) { 
				cur_token.type = reserved_words[cur_symbol];
			}
			switch (cur_token.type) {
				case INCOMPLETE_NUMERIC_CONSTANT_ERROR:
				case UNCLOSED_BLOCK_COMMENT_ERROR:
				case UNCLOSED_CHAR_CONSTANT_ERROR:
				case UNCLOSED_STRING_LITERAL_ERROR:
				case ILLEGAL_CHAR_ERROR:
					lexer.error_token_ids.push_back(lexer.token_stream.size()); 
				case PREPROCESSOR:
				case LINE_COMMENT:
				case BLOCK_COMMENT:
				case IDENTIFIER:
				case NUMERIC_CONSTANT:
				case CHAR_CONSTANT:
				case STRING_LITERAL: {
					int pos(idx_in_symbol_table.search(cur_symbol));
					if (pos == Lexer::NOT_FOUND) {
						pos = symbol_table.size();
						idx_in_symbol_table.insert(cur_symbol, pos);
						symbol_table.push_back(cur_symbol);
					}
					cur_token.symbol_idx = pos;
					break;
				}
				default:
					cur_token.symbol_idx = cur_token.type;
			}
			lexer.token_stream.push_back(cur_token); 
			++lexer.token_type_cnts[cur_token.type]; 
			cur_symbol.clear();
			expects_new_token = true;
		}
		lexer.rowCnt = cur_row;
	}
	lexer.token_stream.emplace_back(END, END, -1, -1);
	/*
	cout << "lexer outputs:\n";
	for (const auto& token : lexer.token_stream) {
		cout << "\t\t\t\t" << token;
	}
	cout << endl;
	*/
	return lexer;
}

/*
vector<Token>& operator >>(Lexer& lexer, vector<Token>& token_stream) {
	token_stream = std::move(lexer.token_stream);
	return token_stream;
}
*/