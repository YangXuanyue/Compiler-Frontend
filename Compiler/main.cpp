#include <iostream>
#include <fstream>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

using std::cout;
using std::cin;

int main() {
	Lexer lexer('\n');
	Parser parser;
	while (true) {
		cout << "input string:\n\t\t\t\t";
		cin >> lexer >> parser;
		//input an empty string to exit
		if (lexer.get_token_stream().front().type == END) { 
			return 0;
		}
	}
}