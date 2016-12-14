#pragma once

#include "Item.h"
#include <set>

using std::set;

namespace LR {
	class Dfa;
	class Parser;

	class ItemSet {
		friend ostream& operator <<(ostream& out, const ItemSet& item_set) {
			for (const auto& item : item_set.items) {
				out << item;
			}
			return out;
		}
		
		friend class Dfa;
		friend class Parser;

		set<Item> items;
		map<Symbol, set<Item>> transitable_items;
		set<Item> reduced_items;

	public:
		ItemSet(const set<Item>& items = {}) : items(items) {
			extend();
			for (const auto& item : this->items) {
				const auto& production(
					grammar.productions[
						item.production_idx
					]
				);
				if (item.dot_pos < production.right.size()) {
					transitable_items[
						production.right[
							item.dot_pos
						]
					].insert(item);
				} else {
					reduced_items.insert(item);
				}
			}
			/*cout << "===\n" << *this << "===\n";
			
			for (const auto& nonterminal : grammar.nonterminals) {
				print_symbol(cout, nonterminal) << ":\n";
				if (transitable_items.find(nonterminal) != transitable_items.end()) {
					for (const auto& item : transitable_items[nonterminal]) {
						cout << item;
					}
				}
			}
			
			*/
		}
		
		ItemSet(const ItemSet& src) : 
			items(src.items), 
			transitable_items(src.transitable_items),
			reduced_items(src.reduced_items) {
		}

		ItemSet(ItemSet&& src) : 
			items(std::move(src.items)),
			transitable_items(std::move(src.transitable_items)),
			reduced_items(std::move(src.reduced_items)) {
		}

		bool has_item(const Item& item) {
			return items.find(item) != items.end();
		}

		void extend() {
			auto new_items(items);
			for (; new_items.size(); ) {
				set<Item> tmp_items;
				for (const auto& item : new_items) {
					const auto& production(
						grammar.productions[
							item.production_idx
						]
					);
					if (item.dot_pos < production.right.size()
						&& production.right[item.dot_pos].which() == NONTERMINAL) {
						const auto& nonterminal(production.right[item.dot_pos]);
						for (int idx : grammar.production_idxes[nonterminal]) {
							Item new_item(idx, 0);
							if (!has_item(new_item)) {
								tmp_items.insert(std::move(new_item));
							}
						}
					}
				}
				items.insert(
					tmp_items.begin(),
					tmp_items.end()
				);
				new_items = std::move(tmp_items);
			}
		}

		bool operator ==(const ItemSet& rhs) const {
			if (items.size() == rhs.items.size()) {
				for (auto it(items.begin()), r_it(rhs.items.begin());
					 it != items.end();
					 ++it, ++r_it) {
					if (*it != *r_it) {
						return false;
					}
				}
				return true;
			}
			return false;
		}
	};
}