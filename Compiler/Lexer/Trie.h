#ifndef LEXER_TRIE_H
#define LEXER_TRIE_H

#include <cstring>
#include <string>

using std::string;

#include <iostream>

using std::cout;
using std::endl;

template <typename T, T DefaultVal>
class Trie {
private:
	enum {
		SIGMA_SIZE = 128
	};
	struct Node {
		int next[SIGMA_SIZE];
		T val;
		
		Node() {
			val = DefaultVal;
			memset(next, 0, sizeof(next));
		}
	};
	Node* nodes;
	int size, root;
	
	int new_node() {
		return size++;
	}

public:
	Trie(int maxSize = 1) {
		nodes = new Node[maxSize];
		size = 0;
		root = new_node();
	}
	
	~Trie() {
		delete [] nodes;
	}
	
	void insert(const string& word, const T& val) {
		int cur(root);
		for (char c : word) {
			if (!nodes[cur].next[c]) {
				nodes[cur].next[c] = new_node();
			}
			cur = nodes[cur].next[c];
		}
		nodes[cur].val = val;
	}
	
	T search(const string& word) {
		int cur(root);
		for (char c : word) {
			if (!nodes[cur].next[c]) {
				return DefaultVal;
			}
			cur = nodes[cur].next[c];
		}
		return nodes[cur].val;
	}
};

#endif //LEXER_TRIE_H
