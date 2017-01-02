#pragma once

#include <iostream>
#include "../Grammar.h"
#include "ErrorMsgs.h"

using std::ostream;

namespace LR {
	enum ActionType {
		SHIFT,
		REDUCE,
		ACC,
		ERROR
	};

	struct Action {
		friend ostream& operator <<(ostream& out, const Action& action) {
			switch (action.type) {
				case SHIFT:
					out << "shift " << action.val << endl;
					break;
				case REDUCE:
					out << "reduce " << grammar.get_productions()[action.val];
					break;
				case ACC:
					out << "acc\n";
					break;
				case ERROR:
					out << "error: " << error_msgs[action.val] << endl;
			}
			return out;
		}

		ActionType type;
		int val;

		Action(ActionType type = SHIFT, int val = -1) : type(type), val(val) {
		}
	};
}
