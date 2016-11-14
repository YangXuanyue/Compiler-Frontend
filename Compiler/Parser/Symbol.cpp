#include "Symbol.h"

void print_symbol(ostream& out, const Symbol& symbol) {
	out << ((symbol.which() == NONTERMINAL)? 
			symbol : token_vals[boost::get<TokenType>(symbol)]);
}