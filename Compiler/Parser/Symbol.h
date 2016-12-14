#pragma once

#include <boost/variant.hpp>
#include <string>
#include "../TokenType.h"
#include <iostream>

using boost::variant;
using std::string;
using std::ostream;

enum SymbolType {
	NONTERMINAL, //string
	TERMINAL //TokenType
};

typedef variant<string, TokenType> Symbol;

ostream& print_symbol(ostream& out, const Symbol& symbol);