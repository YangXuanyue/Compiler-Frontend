#include "Dfa.h"
#include <algorithm>

using std::fill;

Dfa::Dfa() {
	cur = START;
	states[cur].next['#'] = 1;
	states[cur].next['/'] = 2;
	states[cur].next['_'] = 6;
	fill(states[cur].next + 'a', states[cur].next + 'z' + 1, 6);
	fill(states[cur].next + 'A', states[cur].next + 'Z' + 1, 6);
	fill(states[cur].next + '0', states[cur].next + '9' + 1, 7);
	states[cur].next['\''] = 12;
	states[cur].next['\"'] = 15;
	states[cur].next['='] = 17;
	states[cur].next['+'] = 18;
	states[cur].next['-'] = 19;
	states[cur].next['*'] = 20;
	states[cur].next['%'] = 21;
	states[cur].next['&'] = 22;
	states[cur].next['|'] = 24;
	states[cur].next['^'] = 26;
	states[cur].next['~'] = START;
	states[cur].output['~'] = BITWISE_NOT;
	states[cur].next['!'] = 27;
	states[cur].next['<'] = 28;
	states[cur].next['>'] = 30;
	states[cur].next['?'] = START;
	states[cur].output['?'] = QUESTION;
	states[cur].next[','] = START;
	states[cur].output[','] = COMMA;
	states[cur].next[':'] = START;
	states[cur].output[':'] = COLON;
	states[cur].next[';'] = START;
	states[cur].output[';'] = SEMICOLON;
	states[cur].next['.'] = START;
	states[cur].output['.'] = DOT;
	states[cur].next['{'] = START;
	states[cur].output['{'] = L_BRACE;
	states[cur].next['}'] = START;
	states[cur].output['}'] = R_BRACE;
	states[cur].next['('] = START;
	states[cur].output['('] = L_PAREN;
	states[cur].next[')'] = START;
	states[cur].output[')'] = R_PAREN;
	states[cur].next['['] = START;
	states[cur].output['['] = L_SQUARE;
	states[cur].next[']'] = START;
	states[cur].output[']'] = R_SQUARE;
	states[cur].next['@'] = START;
	states[cur].output['@'] = ILLEGAL_CHAR_ERROR;
	states[cur].next['$'] = START;
	states[cur].output['$'] = ILLEGAL_CHAR_ERROR;
	
	cur = 1;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, 1);
	states[cur].next['\n'] = START;
	states[cur].output['\n'] = PREPROCESSOR;
	states[cur].next['#'] = START;
	states[cur].output['#'] = CONCAT;
	
	cur = 2;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, DIV);
	states[cur].retract_flg.set();
	states[cur].output['='] = DIV_ASSIGN;
	states[cur].retract_flg.reset('=');
	states[cur].next['/'] = 3;
	states[cur].output['/'] = UNKNOWN;
	states[cur].retract_flg.reset('/');
	states[cur].next['*'] = 4;
	states[cur].output['*'] = UNKNOWN;
	states[cur].retract_flg.reset('*');
	
	cur = 3;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, 3);
	states[cur].next['\n'] = START;
	states[cur].output['\n'] = LINE_COMMENT;
	states[cur].next['\0'] = START; //EOF
	states[cur].output['\0'] = LINE_COMMENT;
	
	cur = 4;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, 4);
	states[cur].next['*'] = 5;
	states[cur].next['\0'] = START; //EOF
	states[cur].output['\0'] = UNCLOSED_BLOCK_COMMENT_ERROR;
	
	cur = 5;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, 4);
	states[cur].next['*'] = 5;
	states[cur].next['/'] = START;
	states[cur].output['/'] = BLOCK_COMMENT;
	states[cur].next['\0'] = START; //EOF
	states[cur].output['\0'] = UNCLOSED_BLOCK_COMMENT_ERROR;
	
	cur = 6;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, IDENTIFIER);
	states[cur].retract_flg.set();
	states[cur].retract_flg.reset('\n');
	states[cur].retract_flg.reset('\t');
	states[cur].retract_flg.reset(' ');
	states[cur].next['_'] = 6;
	states[cur].output['_'] = UNKNOWN;
	states[cur].retract_flg.reset('_');
	for (char c = 'a'; c <= 'z'; ++c) {
		states[cur].next[c] = 6;
		states[cur].output[c] = UNKNOWN;
		states[cur].retract_flg.reset(c);
	}
	for (char c = 'A'; c <= 'Z'; ++c) {
		states[cur].next[c] = 6;
		states[cur].output[c] = UNKNOWN;
		states[cur].retract_flg.reset(c);
	}
	for (char c = '0'; c <= '9'; ++c) {
		states[cur].next[c] = 6;
		states[cur].output[c] = UNKNOWN;
		states[cur].retract_flg.reset(c);
	}
	
	cur = 7;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, INT_CONSTANT);
	states[cur].retract_flg.set();
	states[cur].retract_flg.reset('\n');
	states[cur].retract_flg.reset('\t');
	states[cur].retract_flg.reset(' ');
	states[cur].next['.'] = 8;
	states[cur].output['.'] = UNKNOWN;
	states[cur].retract_flg.reset('.');
	states[cur].next['e'] = 9;
	states[cur].output['e'] = UNKNOWN;
	states[cur].retract_flg.reset('e');
	states[cur].next['E'] = 9;
	states[cur].output['E'] = UNKNOWN;
	states[cur].retract_flg.reset('E');
	for (char c = '0'; c <= '9'; ++c) {
		states[cur].next[c] = 7;
		states[cur].output[c] = UNKNOWN;
		states[cur].retract_flg.reset(c);
	}
	
	cur = 8;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, REAL_CONSTANT);
	states[cur].retract_flg.set();
	states[cur].retract_flg.reset('\n');
	states[cur].retract_flg.reset('\t');
	states[cur].retract_flg.reset(' ');
	states[cur].next['e'] = 9;
	states[cur].output['e'] = UNKNOWN;
	states[cur].retract_flg.reset('e');
	states[cur].next['E'] = 9;
	states[cur].output['E'] = UNKNOWN;
	states[cur].retract_flg.reset('E');
	for (char c = '0'; c <= '9'; ++c) {
		states[cur].next[c] = 8;
		states[cur].output[c] = UNKNOWN;
		states[cur].retract_flg.reset(c);
	}
	
	cur = 9;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, INCOMPLETE_NUMERIC_CONSTANT_ERROR);
	states[cur].retract_flg.set();
	states[cur].next['+'] = 10;
	states[cur].output['+'] = UNKNOWN;
	states[cur].retract_flg.reset('+');
	states[cur].next['-'] = 10;
	states[cur].output['-'] = UNKNOWN;
	states[cur].retract_flg.reset('-');
	for (char c = '0'; c <= '9'; ++c) {
		states[cur].next[c] = 11;
		states[cur].output[c] = UNKNOWN;
		states[cur].retract_flg.reset(c);
	}
	
	cur = 10;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, INCOMPLETE_NUMERIC_CONSTANT_ERROR);
	states[cur].retract_flg.set();
	for (char c = '0'; c <= '9'; ++c) {
		states[cur].next[c] = 11;
		states[cur].output[c] = UNKNOWN;
		states[cur].retract_flg.reset(c);
	}
	
	cur = 11;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, REAL_CONSTANT);
	states[cur].retract_flg.set();
	states[cur].retract_flg.reset('\n');
	states[cur].retract_flg.reset('\t');
	states[cur].retract_flg.reset(' ');
	for (char c = '0'; c <= '9'; ++c) {
		states[cur].next[c] = 11;
		states[cur].output[c] = UNKNOWN;
		states[cur].retract_flg.reset(c);
	}
	
	cur = 12;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, 14);
	states[cur].next['\\'] = 13;
	states[cur].next['\n'] = START;
	states[cur].output['\n'] = UNCLOSED_CHAR_CONSTANT_ERROR;
	states[cur].next['\0'] = START;
	states[cur].output['\0'] = UNCLOSED_CHAR_CONSTANT_ERROR;
	
	cur = 13;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, UNCLOSED_CHAR_CONSTANT_ERROR);
	states[cur].retract_flg.set();
	for (char c : {'0', 'n', 'r', 't', 'v', 'a', 'b', 'f', '\'', '\"', '\\'}) {
		states[cur].next[c] = 14;
		states[cur].output[c] = UNKNOWN;
		states[cur].retract_flg.reset(c);
	}
	
	cur = 14;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, UNCLOSED_CHAR_CONSTANT_ERROR);
	states[cur].retract_flg.set();
	states[cur].next['\''] = START;
	states[cur].output['\''] = CHAR_CONSTANT;
	states[cur].retract_flg.reset('\'');
	
	cur = 15;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, 15);
	states[cur].next['\\'] = 16;
	states[cur].next['\"'] = START;
	states[cur].output['\"'] = STRING_LITERAL;
	states[cur].next['\n'] = START;
	states[cur].output['\n'] = UNCLOSED_STRING_LITERAL_ERROR;
	states[cur].next['\0'] = START; //EOF
	states[cur].output['\0'] = UNCLOSED_STRING_LITERAL_ERROR;
	
	cur = 16;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, 15);
	states[cur].next['\n'] = START;
	states[cur].output['\n'] = UNCLOSED_STRING_LITERAL_ERROR;
	states[cur].next['\0'] = START; //EOF
	states[cur].output['\0'] = UNCLOSED_STRING_LITERAL_ERROR;
	
	cur = 17;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, ASSIGN);
	states[cur].retract_flg.set();
	states[cur].output['='] = EQUAL;
	states[cur].retract_flg.reset('=');
	
	cur = 18;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, ADD);
	states[cur].retract_flg.set();
	states[cur].output['+'] = INC;
	states[cur].retract_flg.reset('+');
	states[cur].output['='] = ADD_ASSIGN;
	states[cur].retract_flg.reset('=');
	
	cur = 19;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, SUB);
	states[cur].retract_flg.set();
	states[cur].output['-'] = DEC;
	states[cur].retract_flg.reset('-');
	states[cur].output['='] = SUB_ASSIGN;
	states[cur].retract_flg.reset('=');
	states[cur].output['>'] = ARROW;
	states[cur].retract_flg.reset('>');
	
	cur = 20;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, ASTERISK);
	states[cur].retract_flg.set();
	states[cur].output['='] = MUL_ASSIGN;
	states[cur].retract_flg.reset('=');
	
	cur = 21;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, MOD);
	states[cur].retract_flg.set();
	states[cur].output['='] = MOD_ASSIGN;
	states[cur].retract_flg.reset('=');
	
	cur = 22;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, AMPERSAND);
	states[cur].retract_flg.set();
	states[cur].output['='] = BITWISE_AND_ASSIGN;
	states[cur].retract_flg.reset('=');
	states[cur].next['&'] = 23;
	states[cur].output['&'] = UNKNOWN;
	states[cur].retract_flg.reset('&');
	
	cur = 23;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, AND);
	states[cur].retract_flg.set();
	states[cur].output['='] = AND_ASSIGN;
	states[cur].retract_flg.reset('=');
	
	cur = 24;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, BITWISE_OR);
	states[cur].retract_flg.set();
	states[cur].output['='] = BITWISE_OR_ASSIGN;
	states[cur].retract_flg.reset('=');
	states[cur].next['|'] = 25;
	states[cur].output['|'] = UNKNOWN;
	states[cur].retract_flg.reset('|');
	
	cur = 25;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, OR);
	states[cur].retract_flg.set();
	states[cur].output['='] = OR_ASSIGN;
	states[cur].retract_flg.reset('=');
	
	cur = 26;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, BITWISE_XOR);
	states[cur].retract_flg.set();
	states[cur].output['='] = BITWISE_XOR_ASSIGN;
	states[cur].retract_flg.reset('=');
	
	cur = 27;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, NOT);
	states[cur].retract_flg.set();
	states[cur].output['='] = INEUQUAL;
	states[cur].retract_flg.reset('=');
	
	cur = 28;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, LESS);
	states[cur].retract_flg.set();
	states[cur].output['='] = LESS_EQUAL;
	states[cur].retract_flg.reset('=');
	states[cur].next['<'] = 29;
	states[cur].output['<'] = UNKNOWN;
	states[cur].retract_flg.reset('<');
	
	cur = 29;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, SHL);
	states[cur].retract_flg.set();
	states[cur].output['='] = SHL_ASSIGN;
	states[cur].retract_flg.reset('=');

	cur = 30;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, GREATER);
	states[cur].retract_flg.set();
	states[cur].output['='] = GREATER_EQUAL;
	states[cur].retract_flg.reset('=');
	states[cur].next['>'] = 31;
	states[cur].output['>'] = UNKNOWN;
	states[cur].retract_flg.reset('>');

	cur = 31;
	fill(states[cur].next, states[cur].next + SIGMA_SIZE, START);
	fill(states[cur].output, states[cur].output + SIGMA_SIZE, SHR);
	states[cur].retract_flg.set();
	states[cur].output['='] = SHR_ASSIGN;
	states[cur].retract_flg.reset('=');
}
