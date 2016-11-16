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
	friend class Parser;

private:
	vector<Symbol> nonterminals, terminals;
	Symbol start_symbol;
	vector<Production> productions;
	map<Symbol, set<int>> production_idxes;
	vector<set<Symbol>> first_of_production;
	map<Symbol, set<Symbol>> first, follow;
	map<Symbol, bool> has_constructed_first, has_constructed_follow;
	map<Symbol, map<Symbol, bool>> includes_follow_of;

public:
	Grammar();
	~Grammar();
	void load_grammar();
	void print_productions();
	void print_first();
	void print_follow();
	void remove_left_recursion();
	void extract_common_left_factor();
	void construct_first();
	void construct_first(const Symbol& nonterminal);
	void construct_follow();
	void construct_follow(const Symbol& nonterminal);

	const Symbol& get_start_symbol() const {
		//cout << start_symbol << endl;
		return start_symbol;
	}

	const vector<Production>& get_productions() const {
		return productions;
	}
/*
	const vector<Symbol>& get_nonterminals() {
		return nonterminals;
	}

	const vector<Symbol>& get_nonterminals() {
		return nonterminals;
	}

	const map<Symbol, set<Symbol>>& get_first() {
		return first;
	}

	const map<Symbol, set<Symbol>>& get_follow() {
		return follow;
	}
*/
};

