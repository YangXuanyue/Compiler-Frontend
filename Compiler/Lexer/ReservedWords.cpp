#include "ReservedWords.h"

ReservedWords::ReservedWords() {
	trie = new Trie<TokenType, IDENTIFIER>(MAX_TRIE_SIZE);
	for (int type(CHAR); type <= RETURN; ++type) {
		trie->insert(token_vals[type], TokenType(type));
	}
}
