#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <iostream>
#include <string>
#include <vector>
#include "TokenType.h"
#include "SymbolTable.h"

using std::ostream;
using std::string;
using std::vector;

struct Token {
	friend ostream& operator <<(ostream& out, const Token& rhs) {
		out << "<" << token_type_strs[rhs.type] << ", \"";
		if (rhs.type < PREPROCESSOR) {
			out << token_vals[rhs.symbol_pos];
		} else {
			out << symbol_table[rhs.symbol_pos];
		}
		out << "\"> in (" << rhs.row << " : " << rhs.col << ")\n";
		return out;
	}

	TokenType type;
	int symbol_pos;
	int row, col;
	
	Token(TokenType _type = UNKNOWN,
	      int _symbolPos = -1,
	      int _row = 1,
	      int _col = 1) :
		type(_type), symbol_pos(_symbolPos), row(_row), col(_col) {
	}
};

#endif //LEXER_TOKEN_H
