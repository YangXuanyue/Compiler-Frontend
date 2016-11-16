#ifndef LEXER_TOKEN_TYPE_H
#define LEXER_TOKEN_TYPE_H

#include <string>
#include <array>
//#include "Trie.h"

using std::string;
using std::array;

enum {
	TOKEN_TYPE_NUM = 100
};

enum TokenType {
	UNKNOWN = 0,
	
	CHAR,
	INT,
	LONG,
	FLOAT,
	DOUBLE,
	VOID,
	UNSIGNED,
	SIGNED,
	CONST,
	STATIC,
	EXTERN,
	STRUCT,
	UNION,
	TYPEDEF,
	SIZEOF,
	
	IF,
	ELSE,
	DO,
	WHILE,
	FOR,
	SWITCH,
	CASE,
	DEFAULT,
	CONTINUE,
	BREAK,
	GOTO,
	
	RETURN,
	ASSIGN, 
	ADD, 
	INC,
	ADD_ASSIGN, 
	SUB, 
	DEC,
	SUB_ASSIGN,
	MUL_ASSIGN,
	DIV, 
	DIV_ASSIGN,
	MOD, 
	MOD_ASSIGN,
	BITWISE_AND_ASSIGN,
	BITWISE_OR, 
	BITWISE_OR_ASSIGN,
	BITWISE_XOR, 
	BITWISE_XOR_ASSIGN,
	BITWISE_NOT, 
	SHL,
	SHL_ASSIGN,
	SHR,
	SHR_ASSIGN,
	AND, 
	AND_ASSIGN,
	OR, 
	OR_ASSIGN,
	NOT, 
	LESS, 
	LESS_EQUAL, 
	EQUAL, 
	INEUQUAL, 
	GREATER, 
	GREATER_EQUAL, 
	CONCAT,
	ASTERISK,
	AMPERSAND,
	QUESTION,
	COMMA,
	COLON,
	SEMICOLON,
	DOT,
	ARROW,
	L_BRACE,
	R_BRACE,
	L_SQUARE,
	R_SQUARE,
	L_PAREN,
	R_PAREN,
	
	END,
	EPSILON,

	PREPROCESSOR,
	LINE_COMMENT,
	BLOCK_COMMENT,
	IDENTIFIER,
	NUMERIC_CONSTANT,
	CHAR_CONSTANT,
	STRING_LITERAL,
	INCOMPLETE_NUMERIC_CONSTANT_ERROR,
	UNCLOSED_BLOCK_COMMENT_ERROR,
	UNCLOSED_CHAR_CONSTANT_ERROR,
	UNCLOSED_STRING_LITERAL_ERROR,
	ILLEGAL_CHAR_ERROR,
};

extern const array<string, TOKEN_TYPE_NUM> token_type_strings;
extern const array<string, TOKEN_TYPE_NUM> token_vals;

template <typename V, V DefaultVal>
class Trie;

//quickly converts a token_val string to its corresponding TokenType
class StringToTokenType {
private:
	enum {
		MAX_TRIE_SIZE = 500
	};
	Trie<TokenType, IDENTIFIER>* trie;

public:
	StringToTokenType(TokenType first_type, TokenType last_type);
	~StringToTokenType();
	TokenType operator [](const string& word) const;
};

extern StringToTokenType string_to_token_type;
extern StringToTokenType reserved_words;

#endif