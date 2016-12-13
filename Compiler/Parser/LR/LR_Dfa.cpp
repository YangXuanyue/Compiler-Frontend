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
		ItemSet start_item_set{{start_production_idx, 0}};
		item_sets.emplace_back(std::move(start_item_set));
		item_sets.back().extend();
		//bfs

	}
}