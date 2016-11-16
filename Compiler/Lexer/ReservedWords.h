#ifndef LEXER_RESERVEDWORDS_H
#define LEXER_RESERVEDWORDS_H

#include "../Trie.h"
#include "../TokenType.h"

class ReservedWords {
private:
	enum {
		MAX_TRIE_SIZE = 300
	};
	Trie<TokenType, IDENTIFIER>* trie;

public:
	ReservedWords() {
		trie = new Trie<TokenType, IDENTIFIER>(MAX_TRIE_SIZE);
		for (int type(CHAR); type <= RETURN; ++type) {
			trie->insert(token_vals[type], TokenType(type));
		}
	}
	
	~ReservedWords() {
		delete trie;
	}
	
	TokenType search(const string& word) {
		return trie->search(word);
	}
};

#endif //LEXER_RESERVEDWORDS_H
