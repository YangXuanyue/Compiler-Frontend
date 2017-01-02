#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/variant.hpp>
#include "TokenType.h"
#include "SymbolTable.h"

using std::ostream;
using std::string;
using std::vector;
using boost::variant;

struct Token {
	friend ostream& operator <<(ostream& out, const Token& rhs) {
		out << "<" << token_type_strs[rhs.type] << ", \"";
		if (rhs.type < PREPROCESSOR) {
			out << token_vals[rhs.symbol_idx];
		} else {
			out << symbol_table[rhs.symbol_idx];
		}
		out << "\"> in (" << rhs.row << " : " << rhs.col << ")\n";
		return out;
	}

	TokenType type;
	int symbol_idx;
	int row, col;
	variant<long long, double> val;
	
	Token(TokenType _type = UNKNOWN,
	      int _symbolPos = -1,
	      int _row = 1,
	      int _col = 1,
		  const variant<long long, double>& _val = 0.) :
		type(_type), symbol_idx(_symbolPos), row(_row), col(_col), val(_val) {
	}
};

#endif //LEXER_TOKEN_H
