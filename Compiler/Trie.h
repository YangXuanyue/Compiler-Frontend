#pragma once

#include <cstring>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include "Parser/Grammar.h" //for friend declaration

using std::vector;
using std::string;
using std::set;
using std::cout;
using std::endl;

enum {
	DEFAULT_TRIE_SIGMA_SIZE = 128
};

template <typename V, V DefaultVal>
class Trie {
	//extracting common left factor of a grammar needs to
	//hack into a trie's inside stucture
	friend void Grammar::extract_common_left_factor();

private:
	struct Node {
		vector<int> next;
		V val;
		set<V> vis_vals;

		void clear(int sigma_size) {
			val = DefaultVal;
			vis_vals.clear();
			next.assign(sigma_size, 0);
		}
	};
	int sigma_size;
	vector<Node> nodes;
	int size, root;
	
	int new_node() {
		nodes[size].clear(sigma_size);
		return size++;
	}

public:
	Trie(int max_size = 1, int _sigma_size = DEFAULT_TRIE_SIGMA_SIZE) :
		sigma_size(_sigma_size) {
		nodes.resize(max_size);
		clear();
	}

	void clear() {
		size = 0;
		root = new_node();
	}

	template <typename String = string, typename Char = char>
	void insert(const String& word, const V& val) {
		int cur(root);
		for (Char c : word) {
			if (!nodes[cur].next[c]) {
				nodes[cur].next[c] = new_node();
			}
			cur = nodes[cur].next[c];
			nodes[cur].vis_vals.insert(val);
		}
		nodes[cur].val = val;
	}

	template <typename String = string, typename Char = char>
	void erase(const String& word, const V& val) {
		int cur(root);
		for (Char c : word) {
			cur = nodes[cur].next[c];
			nodes[cur].vis_vals.erase(val);
		}
	}
	
	template <typename String = string, typename Char = char>
	V search(const String& word) {
		int cur(root);
		for (Char c : word) {
			if (!nodes[cur].next[c]) {
				return DefaultVal;
			}
			cur = nodes[cur].next[c];
		}
		return nodes[cur].val;
	}
};

