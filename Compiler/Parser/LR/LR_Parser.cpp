#include "LR_Parser.h"

namespace LR {
	Parser& operator >> (const Lexer& lexer, Parser& parser) {
		// TODO: insert return statement here

		return parser;
	}

	void Parser::construct_parsing_table() {
		grammar.augment();
	}
}
