#pragma once

#include "../Grammar.h"
#include "ItemSet.h"

namespace LR {
	class Parser;

	class Dfa {
		friend class Parser;

	private:
		vector<ItemSet> item_sets; 
		struct State {
			map<Symbol, int> next;

			State() {
			}
		};
		vector<State> states;
		int state_num;

		Dfa();

	public:
		enum {
			START = 0
		};

		static Dfa& get_instance() {
			static Dfa instance;
			return instance;
		}
	};

	#define dfa Dfa::get_instance()
}