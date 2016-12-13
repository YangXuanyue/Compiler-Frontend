#pragma once

#include "../../Token.h"
#include "../../Lexer/Lexer.h"
#include "../Grammar.h"
#include "Action.h"
#include <vector>
#include <map>

using std::vector;
using std::map;

namespace LR {
	class Parser {
		friend Parser& operator >>(const Lexer& lexer, Parser& parser);

	private:
		vector<map<Symbol, Action>> action_table;
		vector<map<Symbol, int>> goto_table;

		void construct_parsing_table();

	public:
		Parser() {
			construct_parsing_table();
		}
	};
}