#include <iostream>
#include <string>
#include <sstream>
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
	}
	return 0;
}