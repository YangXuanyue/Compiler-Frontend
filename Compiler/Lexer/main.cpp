#include <iostream>
#include <fstream>
#include "Lexer.h"
#include <cstdlib>

using namespace std;

const string DEFAULT_IN_FILE("c_code.txt");
const string DEFAULT_OUT_FILE("token_stream.txt");

int main(int argc, char* argv[]) {
	//system("clang -cc1 -dump-tokens c_code.txt");
	Lexer lexer;
	string inFile = (argc > 1) ? argv[1] : DEFAULT_IN_FILE;
	string outFile = (argc > 2) ? argv[2] : DEFAULT_OUT_FILE;
	ifstream in(inFile);
	ofstream out(outFile);
	if (in.is_open() && out.is_open()) {
		in >> lexer;
		out << lexer;
		out.close();
		system(outFile.c_str());
	} else {
		cout << "cannot open file\n";
	}
	system("pause");
	return 0;
}