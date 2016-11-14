#if 0

#include "Lexer.h"
#include <fstream>

const string IN_FILE("input.txt");
const string OUT_FILE("output.txt");

using namespace std;

	template <typename T, T DefaultVal>
	class Trie {
		private:
		enum {
			SIGMA_SIZE = 128 //字母表大小
		};
		struct Node { //结点
			int next[SIGMA_SIZE]; //读取一个字符后转移到的下一个状态
			T val; //该节点上存储的信息
		};
		Node* nodes; //结点表
		int size; //结点数目
		int root; //根节点标号

		public:
		//将一个带val信息的字符串插入Trie树中
		void insert(const string& word, const T& val);
		//从Trie树中查询某一个字符串所带信息
		T search(const string& word);
	};

	struct Token {
		TokenType type; //类型
		int symbolPos; //表示的符号在符号表中位置
		int row, col; //在代码中的行列位置
	};

	enum TokenType {
		UNKNOWN,

		CHAR,
		//...
		IF,
		//...
		ADD,
		//...
		PREPROCESSOR,
		IDENTIFIER,
		//...
	};

	const array<string, TOKEN_TYPE_NUM> tokenTypeStrs = {
		"UNKNOWN",
		"CHAR",
		//...
	};

	const array<string, TOKEN_TYPE_NUM> tokenVals = {
		"unknown",
		"char",
		//...
	};

	class DFA {
		enum {
			STATE_NUM = 50, //状态数
			SIGMA_SIZE = 128, //字母表大小
		};
		struct State {
			int next[SIGMA_SIZE]; //读取一个字母后转移到的下一个状态
			TokenType output[SIGMA_SIZE]; //读取一个字母后输出已识别的token类型
			bitset<SIGMA_SIZE> retractFlg; //标记读取一个字母后是否需回退
		} states[STATE_NUM];
		int cur; //当前状态标号
		TokenType output; //当前输出
		bool retractFlg; //当前回退标记

		public:
		enum {
			START = 0 //起始状态标号
		};
		//初始化
		void init(); 
		//读取字母c进行状态转移
		void trans(char c);
		//获取当前状态标号
		int getState();
		//获取当前输出
		TokenType getOutput();
		//判断当前是否需回退
		bool needsRetract();
	};

	class Lexer {
		friend istream& operator >> (istream& in, Lexer& rhs); //输入流操作符
		friend ostream& operator <<(ostream& out, const Lexer& rhs); //输出流操作符

		private:
		static DFA* dfa; //DFA
		static ReservedWords* reservedWords; //保留字表
		vector<Token> tokens; //识别出的token流
		vector<string> symbols; //符号表
		Trie<int, -1>* posInSymbols; //查询当前符号在符号表中位置的Trie
		int rowCnt; //行计数器
		int charCnt; //字符计数器
		array<int, TOKEN_TYPE_NUM> tokenTypeCnts; //各类token的计数器
		vector<int> errorTokenIds; //错误token标号
	};

	#include "Lexer.h"

	istream& operator >> (istream& in, Lexer& rhs) {
		rhs.tokens.clear();
		rhs.symbols.clear();
		rhs.posInSymbols = new Trie<int, -1>(Lexer::MAX_TRIE_SIZE);
		rhs.dfa->init();
		rhs.charCnt = 0;
		fill(rhs.tokenTypeCnts.begin(), rhs.tokenTypeCnts.end(), 0);
		rhs.errorTokenIds.clear();
		Token curToken;
		string curSymbol;
		int curRow(1), curCol(1);
		bool reachesEOF = false;
		bool expectsNewToken = true;
		for (char c; !reachesEOF; ) {
			if ((c = in.get()) == EOF) {
				//for recognition of token just before EOF
				//since '\0' does not exist in the C-code text, 
				//use it to substitute EOF = -1
				c = '\0';
				reachesEOF = true;
			}
			if (expectsNewToken && !isBlankChar(c)) {
				curToken.row = curRow;
				curToken.col = curCol;
				expectsNewToken = false;
			}
			Lexer::dfa->trans(c);
			if (Lexer::dfa->needsRetract()) {
				in.putback(c);
			} else {
				if (Lexer::dfa->getState() != DFA::START) {
					curSymbol += c;
				} else if (!isBlankChar(c)) {
					curSymbol += c;
				}
				++rhs.charCnt;
				++curCol;
				if (c == '\n') {
					++curRow;
					curCol = 1;
				}
			}
			if (Lexer::dfa->getOutput() != UNKNOWN) {
				curToken.type = Lexer::dfa->getOutput();
				if (curToken.type == IDENTIFIER) {
					curToken.type = Lexer::reservedWords->search(curSymbol);
				}
				switch (curToken.type) {
					case INCOMPLETE_NUMERIC_CONSTANT_ERROR:
					case UNCLOSED_BLOCK_COMMENT_ERROR:
					case UNCLOSED_CHAR_CONSTANT_ERROR:
					case UNCLOSED_STRING_LITERAL_ERROR:
					case ILLEGAL_CHAR_ERROR:
						rhs.errorTokenIds.push_back(rhs.tokens.size());
					case PREPROCESSOR:
					case LINE_COMMENT:
					case BLOCK_COMMENT:
					case IDENTIFIER:
					case NUMERIC_CONSTANT:
					case CHAR_CONSTANT:
					case STRING_LITERAL:
					{
						int pos(rhs.posInSymbols->search(curSymbol));
						if (!~pos) {
							pos = rhs.symbols.size();
							rhs.posInSymbols->insert(curSymbol, pos);
							rhs.symbols.push_back(curSymbol);
						}
						curToken.symbolPos = pos;
						break;
					}
					default:
						curToken.symbolPos = curToken.type;
				}
				rhs.tokens.push_back(curToken);
				++rhs.tokenTypeCnts[curToken.type];
				curSymbol.clear();
				expectsNewToken = true;
			}
			rhs.rowCnt = curRow;
		}
		return in;

void f(){


	Lexer lexer; //词法分析器
	ifstream in(IN_FILE); //输入文件为C语言代码
	ofstream out(OUT_FILE); //输出文件为经分析得到的token流
	in >> lexer;
	out << lexer;

}

#endif