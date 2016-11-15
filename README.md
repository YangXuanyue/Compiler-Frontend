# Compiler
A far-from-complete C compiler, including a lexer and an expression parser so far, project for course Compiler Principle and Technology.
##Lexer
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
##Parser
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
