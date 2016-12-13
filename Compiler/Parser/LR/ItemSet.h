#pragma once

#include "Item.h"
#include <set>

using std::set;

namespace LR {
	class ItemSet {
		friend ostream& operator <<(ostream& out, const ItemSet& itemset) {
			for (const auto& item : items) {
				out << item;
			}
		}

		set<Item> items;

	public:
		ItemSet(const set<Item>& items = {}) : items(items) {
		}
		
		ItemSet(const ItemSet& src) : ItemSet(src.items) {
		}

		ItemSet(ItemSet&& src) : items(std::move(src.items)) {
		}

		bool has_item(const Item& item) {
			return items.find(item) != items.end();
		}
		
		/*
		void insert(const Item& item) {
			items.insert(item);
		}

		void insert(Item&& item) {
			items.insert(std::move(item));
		}
		*/

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
					return true;
				}
			}
			return false;
		}
	};
}