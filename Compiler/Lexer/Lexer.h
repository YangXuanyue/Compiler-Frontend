#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <iostream>
#include <vector>
#include <string>
#include "../Token.h"
//#include "ReservedWords.h"
#include "DFA.h"

using std::istream;
using std::ostream;
using std::vector;

class Lexer {
	friend const Lexer& operator >>(istream& in, Lexer& lexer);
	
private:
	enum {
		MAX_TRIE_SIZE = 10000,
		NOT_FOUND = -1
	};
	int end_char;
	static DFA* dfa;
	vector<Token> token_stream;
	int rowCnt, char_cnt;
	array<int, TOKEN_TYPE_NUM> token_type_cnts;
	vector<int> error_token_ids;

public:
	Lexer(char _end_char = EOF);
	
	~Lexer() {
	}

	const vector<Token>& get_token_stream() const {
		return token_stream;
	}
};

#endif //LEXER_LEXER_H
