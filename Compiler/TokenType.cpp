#include "TokenType.h"

const array<string, TOKEN_TYPE_NUM> token_type_strs = {
	"UNKNOWN",
	"CHAR",
	"INT",
	"LONG",
	"FLOAT",
	"DOUBLE",
	"VOID",
	"UNSIGNED",
	"SIGNED",
	"CONST",
	"STATIC",
	"EXTERN",
	"STRUCT",
	"UNION",
	"TYPEDEF",
	"SIZEOF",
	"IF",
	"ELSE",
	"DO",
	"WHILE",
	"FOR",
	"SWITCH",
	"CASE",
	"DEFAULT",
	"CONTINUE",
	"BREAK",
	"GOTO",
	"RETURN",
	"ASSIGN",
	"ADD",
	"INC",
	"ADD_ASSIGN",
	"SUB",
	"DEC",
	"SUB_ASSIGN",
	"MUL_ASSIGN",
	"DIV",
	"DIV_ASSIGN",
	"MOD",
	"MOD_ASSIGN",
	"BITWISE_AND_ASSIGN",
	"BITWISE_OR",
	"BITWISE_OR_ASSIGN",
	"BITWISE_XOR",
	"BITWISE_XOR_ASSIGN",
	"BITWISE_NOT",
	"SHL",
	"SHL_ASSIGN",
	"SHR",
	"SHR_ASSIGN",
	"AND",
	"AND_ASSIGN",
	"OR",
	"OR_ASSIGN",
	"NOT",
	"LESS",
	"LESS_EQUAL",
	"EQUAL",
	"INEQUAL",
	"GREATER",
	"GREATER_EQUAL",
	"CONCAT",
	"ASTERISK",
	"AMPERSAND",
	"QUESTION",
	"COMMA",
	"COLON",
	"SEMICOLON",
	"DOT",
	"ARROW",
	"L_BRACE",
	"R_BRACE",
	"L_SQUARE",
	"R_SQUARE",
	"L_PAREN",
	"R_PAREN",
	"END",
	"EPSILON",
	"PREPROCESSOR",
	"LINE_COMMENT",
	"BLOCK_COMMENT",
	"IDENTIFIER",
	//"NUMERIC_CONSTANT",
	"INT_CONSTANT",
	"REAL_CONSTANT",
	"CHAR_LITERAL",
	"STRING_LITERAL",
	"INCOMPLETE_NUMERIC_CONSTANT_ERROR",
	"UNCLOSED_BLOCK_COMMENT_ERROR",
	"UNCLOSED_CHAR_CONSTANT_ERROR",
	"UNCLOSED_STRING_LITERAL_ERROR",
	"ILLEGAL_CHAR_ERROR",
};

const array<string, TOKEN_TYPE_NUM> token_vals = {
	"unknown",
	"char",
	"int",
	"long",
	"float",
	"double",
	"void",
	"unsigned",
	"signer",
	"const",
	"static",
	"extern",
	"struct",
	"union",
	"typedef",
	"sizeof",
	"if",
	"else",
	"do",
	"while",
	"for",
	"switch",
	"case",
	"default",
	"continue",
	"break",
	"goto",
	"return",
	"=",
	"+",
	"++",
	"+=",
	"-",
	"--",
	"-=",
	"*=",
	"/",
	"/=",
	"%",
	"%=",
	"&=",
	"|",
	"|=",
	"^",
	"^=",
	"~",
	"<<",
	"<<=",
	">>",
	">>=",
	"&&",
	"&&=",
	"||",
	"||=",
	"!",
	"<",
	"<=",
	"==",
	"!=",
	">",
	">=",
	"##",
	"*",
	"&",
	"?",
	",",
	":",
	";",
	".",
	"->",
	"{",
	"}",
	"[",
	"]",
	"(",
	")",
	"end",
	"epsilon",
	"preprocessor",
	"line_coment",
	"block_comment",
	"id",
	//"num",
	"int_num",
	"real_num",
	"char_constant",
	"string_literal",
	"INCOMPLETE_NUMERIC_CONSTANT_ERROR",
	"UNCLOSED_BLOCK_COMMENT_ERROR",
	"UNCLOSED_CHAR_CONSTANT_ERROR",
	"UNCLOSED_STRING_LITERAL_ERROR",
	"ILLEGAL_CHAR_ERROR",
};

#include "Trie.h"

StringToTokenType::StringToTokenType(TokenType first_type, TokenType last_type) {
	trie = new Trie<TokenType, IDENTIFIER>(MAX_TRIE_SIZE);
	for (int type(first_type); type <= last_type; ++type) {
		trie->insert(token_vals[type], TokenType(type));
	}
}

StringToTokenType::~StringToTokenType() {
	delete trie;
}

TokenType StringToTokenType::operator [](const string& word) const {
	return trie->search(word);
}

StringToTokenType string_to_token_type(UNKNOWN, ILLEGAL_CHAR_ERROR);
StringToTokenType reserved_words(CHAR, RETURN);