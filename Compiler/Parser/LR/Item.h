#pragma once

#include <iostream>
#include "../Grammar.h"

using namespace std;

namespace LR {
	struct Item {
		int production_idx;
		int dot_pos;

		friend ostream& operator <<(ostream& out, const Item& item) {
			const auto& production(
				grammar.get_productions()[
					item.production_idx
				]
			);
			out << production.left << " -> ";
			int cur_pos(0);
			for (const auto& symbol : production.right) {
				if (cur_pos++ == item.dot_pos) {
					out << " ` ";
				}
				print_symbol(out, symbol);
				out << " ";
			}
			if (cur_pos++ == item.dot_pos) {
				out << " ` ";
			}
			out << endl;
			return out;
		}

		Item(int production_idx = -1, int dot_pos = -1) :
			production_idx(production_idx), dot_pos(dot_pos) {
		}

		Item(const Item& src) : Item(src.production_idx, src.dot_pos) {
		}

		Item(Item&& src) :
			production_idx(std::move(src.production_idx)),
			dot_pos(std::move(src.dot_pos)) {
		}

		bool operator <(const Item& rhs) const {
			return ((production_idx != rhs.production_idx
					 && production_idx < rhs.production_idx)
					|| (production_idx == rhs.production_idx
						&& dot_pos < rhs.dot_pos));
		}

		bool operator ==(const Item& rhs) const {
			return (production_idx == rhs.production_idx 
					&& dot_pos == rhs.dot_pos);
		}

		bool operator !=(const Item& rhs) const {
			return !(*this == rhs);
		}
	};
}
