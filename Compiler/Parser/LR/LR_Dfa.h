#pragma once

#include "../Grammar.h"
#include "ItemSet.h"

namespace LR {
	class Dfa {
	private:
		vector<ItemSet> item_sets; 
		
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