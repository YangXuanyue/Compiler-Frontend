#ifndef LEXER_DFA_H
#define LEXER_DFA_H

#include <cstring>
#include <bitset>
#include "../TokenType.h"

using std::bitset;

class Dfa {
	enum {
		STATE_NUM = 50,
		SIGMA_SIZE = 128,
	};
	struct State {
		int next[SIGMA_SIZE];
		TokenType output[SIGMA_SIZE];
		bitset<SIGMA_SIZE> retract_flg;
		
		State() {
			memset(next, 0, sizeof(next));
			memset(output, 0, sizeof(output));
			retract_flg.reset();
		}
	} states[STATE_NUM];
	int cur;
	TokenType output;
	bool retract_flg;

	Dfa();

public:
	static Dfa& get_instance() {
		static Dfa instance;
		return instance;
	}

	enum{
		START = 0
	};
	
	void init() {
		cur = START;
		output = UNKNOWN;
		retract_flg = false;
	}
	
	void transit(char c) {
		output = states[cur].output[c];
		retract_flg = states[cur].retract_flg[c];
		cur = states[cur].next[c];
	}
	
	int get_state() {
		return cur;
	}
	
	TokenType get_output() const {
		return output;
	}
	
	bool needs_retract() {
		return retract_flg;
	}
};

#define dfa Dfa::get_instance()

#endif //LEXER_DFA_H
