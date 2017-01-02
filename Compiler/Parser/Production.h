#pragma once

#include <iostream>
#include <queue>
#include <functional>
#include <tuple>
#include <boost/variant.hpp>
#include "Symbol.h"

using std::ostream;
using std::endl;
using std::deque;
using std::function;
using std::tuple;
using boost::variant;

enum {
	STATE_IDX,
	SYMBOL,
	VAL,
	TYPE
};
typedef tuple<int, Symbol, variant<long long, double>, TokenType> LrStackItem;
typedef function<void(deque<LrStackItem>&)> Translate;

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
	Translate translate;

	Production(const Symbol& _left = "",
			   const deque<Symbol>& _right = {},
			   const Translate& _translate = [](deque<LrStackItem>&) {}) :
		left(_left), right(_right), translate(_translate) {
	}

	Production(Symbol&& _left = "",
			   deque<Symbol>&& _right = {},
			   Translate&& _translate = [](deque<LrStackItem>&) {}) :
		left(std::move(_left)), right(std::move(_right)), translate(std::move(_translate)) {
	}

	Production(const Production& src) :
		left(src.left), right(src.right), translate(src.translate) {
	}

	Production(Production&& src) : 
		left(std::move(src.left)), right(std::move(src.right)), translate(std::move(src.translate)) {
	}
};
