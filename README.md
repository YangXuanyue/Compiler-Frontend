# Compiler
A far-from-complete C compiler, including a lexer and an expression parser so far, project for course `Compiler Principle and Technology`.  For details please refer to [Lab Report for Lexer Design and Implementation](https://github.com/YangXuanyue/Compiler/blob/master/%E8%AF%8D%E6%B3%95%E5%88%86%E6%9E%90%E5%AE%9E%E9%AA%8C%E6%8A%A5%E5%91%8A.pdf) and [Lab Report for LL(1) Parser Design and Implementation](https://github.com/YangXuanyue/Compiler/blob/master/%E8%AF%AD%E6%B3%95%E5%88%86%E6%9E%90%E5%AE%9E%E9%AA%8C%E6%8A%A5%E5%91%8A.pdf).


## Lexer
`Constructor`
```Cpp
//constructs a lexer which stops analysis at end_char, typically EOF or '\n'
Lexer(char end_char = EOF); 
```
`Input`
```Cpp
//returns a ref of lexer in order to be followed by a parser
//nothing more than a grammar sugar
const Lexer& operator >>(std::istream& in, Lexer& lexer); 
```
`Example`
```Cpp
Lexer lexer;
std::cin >> lexer;
```

## Grammar Configuration
The grammar for parsing C codes hasn't completed yet. Nevertheless, it can now be configured in  `../Compiler/Parser/Grammar.ini` in the form of:

	nonterminals = {
		E T F ...
	}
	
	terminals = {
		+ / if for ...
	}
	
	start_symbol = {E}
	
	productions = {
		{E -> E+T | E-T | ...}
		...
	}


## Parser
`Input`
```Cpp
Parser& operator >>(const Lexer& lexer, Parser& parser);
```
`Example`
```Cpp
Lexer lexer;
Parser parser;
std::cin >> lexer >> parser;
```
