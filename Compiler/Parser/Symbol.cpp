#include "Symbol.h"

ostream& print_symbol(ostream& out, const Symbol& symbol) {
	out << ((symbol.which() == NONTERMINAL)? 
			symbol : token_vals[boost::get<TokenType>(symbol)]);
	return out;
}