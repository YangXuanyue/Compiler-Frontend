#pragma once

#include <iostream>
#include <queue>
#include "Symbol.h"

using std::ostream;
using std::endl;
using std::deque;

struct Production {
	friend ostream& operator <<(ostream& out, const Production& production) {
		out << production.left << " -> ";
		for (const auto& symbol : production.right) {
			print_symbol(out, symbol);
			out << " ";
		}
		out << endl;
		return out;
	}
	
	Symbol left;
	deque<Symbol> right;

	Production(const Symbol& _left = "",
			   const deque<Symbol>& _right = {}) :
		left(_left), right(_right) {
	}

	Production(const Production& src) :
		left(src.left), right(src.right) {
	}

	Production(Production&& src) : 
		left(std::move(src.left)), right(std::move(src.right)) {
	}
};
