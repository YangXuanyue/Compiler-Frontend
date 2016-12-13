#pragma once

#include "../../Token.h"
#include "../../Lexer/Lexer.h"
#include "../Grammar.h"
#include <map>

using std::map;

namespace LL {
	class Parser {
		friend Parser& operator >>(const Lexer& lexer, Parser& parser);

		private:
			map<Symbol, map<Symbol, int>> parsing_table;

			void construct_parsing_table();

		public:
			enum {
				SYNCH = -1
			};

			Parser() {
				construct_parsing_table();
			}
	};
}