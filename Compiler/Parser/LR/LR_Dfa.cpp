#include "LR_Dfa.h"
#include <queue>

using std::queue;

namespace LR {
	Dfa::Dfa() {
		int cur(START);
		int start_production_idx(
			*grammar.production_idxes[
				grammar.start_symbol
			].begin()
		);
		ItemSet start_item_set({Item(start_production_idx, 0)});
		//cout << start_item_set << endl;
		item_sets.emplace_back(std::move(start_item_set));
		states.push_back({});
		//bfs
		queue<int> q;
		q.push(START);
		while (q.size()) {
			cur = q.front();
			q.pop();
			for (const auto& symbol_items : item_sets[cur].transitable_items) {
				const Symbol& symbol(symbol_items.first);
				const set<Item>& items(symbol_items.second);
				set<Item> new_items;
				for (const auto& item : items) {
					Item new_item(item);
					++new_item.dot_pos;
					new_items.insert(std::move(new_item));
				}
				ItemSet new_item_set(new_items);
				auto res(
					std::find(item_sets.begin(), item_sets.end(), new_item_set)
				);
				if (res == item_sets.end()) {
					states[cur].next[symbol] = item_sets.size();
					q.push(item_sets.size());
					item_sets.emplace_back(std::move(new_item_set));
					states.push_back({});
				} else {
					states[cur].next[symbol] = res - item_sets.begin();
				}
			}
		}
		state_num = states.size();
		/*for (const auto& item_set : item_sets) {
			cout << "===\n" << item_set << "===\n";
		}*/
	}
}