#if 0
#pragma once

#include <boost/variant.hpp>
#include <string>
#include "TokenType.h"
#include <iostream>

using boost::variant;

using std::string;

using std::ostream;

enum SymbolType {
	NONTERMINAL, //string
	TERMINAL //TokenType
};

typedef variant<string, TokenType> Symbol;

void print_symbol(ostream& out, const Symbol& symbol);

#pragma once

#include <iostream>
#include <queue>
#include "Symbol.h"

using std::ostream;
using std::endl;

using std::deque;

struct Production {
	Symbol left;
	deque<Symbol> right;
};

#pragma once

#include <vector>
#include "Symbol.h"
#include "Production.h"
#include <map>
#include <set>
#include <iostream>

class Parser;

using std::vector;
using std::map;
using std::set;
using std::cout;
using std::endl;

	class Grammar {
	private:
		//非终结符集与终结符集
		vector<Symbol> nonterminals, terminals;
		//起始符号
		Symbol start_symbol;
		//产生式集
		vector<Production> productions;
		//每个非终结符为左端的产生式序号集
		map<Symbol, set<int>> production_idxes;
		//每个产生式右端文法符号串的FIRST集
		vector<set<Symbol>> first_of_production;
		//FIRST集与FOLLOW集
		map<Symbol, set<Symbol>> first, follow;
		//对每个非终结符是否已构造完FIRST集与FOLLOW集的标志，方便递归
		map<Symbol, bool> has_constructed_first, has_constructed_follow;

	public:
		Grammar();
		//消除左递归
		void remove_left_recursion();
		//提取左公因子
		void extract_common_left_factor();
		//对非终结符nonterminal构造FIRST集
		void construct_first(const Symbol& nonterminal);
		//对非终结符nonterminal构造FOLLOW集
		void construct_follow(const Symbol& nonterminal);
	};




#endif // 1
