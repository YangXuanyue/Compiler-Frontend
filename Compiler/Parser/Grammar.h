#pragma once

#include <vector>
#include <queue>
#include "Symbol.h"
#include "Production.h"
#include <map>
#include <set>
#include <iostream>

namespace LL {
	class Parser;
}

namespace LR {
	struct Item;
	class ItemSet;
	class Dfa;
	class Parser;
}

using std::vector;
using std::deque;
using std::map;
using std::set;
using std::cout;
using std::endl;

class Grammar {
	friend class LL::Parser;
	friend struct LR::Item;
	friend class LR::ItemSet;
	friend class LR::Dfa;
	friend class LR::Parser;

private:
	deque<Symbol> nonterminals, terminals;
	Symbol start_symbol;
	vector<Production> productions;
	map<Symbol, set<int>> production_idxes;
	vector<set<Symbol>> first_of_production;
	map<Symbol, set<Symbol>> first, follow;
	map<Symbol, bool> has_constructed_first, has_constructed_follow;
	map<Symbol, map<Symbol, bool>> includes_first_of, includes_follow_of;

	Grammar();

public:
	static Grammar& get_instance() {
		static Grammar instance;
		return instance;
	}

	void load_from_ini();
	void print_productions();
	void print_first();
	void print_follow();
	void augment();
	void remove_epsilon_production();
	void remove_left_recursion();
	void extract_common_left_factor();
	void construct_first();
	void construct_first(const Symbol& nonterminal);
	void construct_follow();
	void construct_follow(const Symbol& nonterminal);

	const Symbol& get_start_symbol() const {
		return start_symbol;
	}

	const vector<Production>& get_productions() const {
		return productions;
	}
/*
	const vector<Symbol>& get_nonterminals() {
		return nonterminals;
	}

	const vector<Symbol>& get_terminals() {
		return terminals;
	}

	const map<Symbol, set<Symbol>>& get_first() {
		return first;
	}

	const map<Symbol, set<Symbol>>& get_follow() {
		return follow;
	}
*/
};

#define grammar Grammar::get_instance()

extern inline bool is_int_or_real(TokenType type);