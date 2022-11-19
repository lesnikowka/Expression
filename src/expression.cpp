#include "expression.h"

expression::expression(std::string str) : infix_str(str) {}
expression::expression(const expression& ex) : infix_str(ex.infix_str), infix(ex.infix), postfix(ex.postfix) {}

std::vector<std::string> expression::get_infix() { return infix; }
std::vector<std::string> expression::get_postfix() { return postfix; }

bool expression::check_brackets() {
	std::stack<char> stack;

	for (char element : infix_str) {
		if (element == special_signes[1] || element == special_signes[2]) {
			if (element == special_signes[1])
				stack.push(special_signes[2]);
			else if (!stack.empty() && stack.top() == element)
				stack.pop();
			else
				return false;
		}
	}
	return stack.empty();
}

bool expression::split() {
	states_of_waiting state = states_of_waiting::number;
	std::vector<std::string> tmp_split;

	if (!check_brackets()) return false;

	for (size_t i = 0; i < infix_str.size(); i++) {
		switch (state) {

		case states_of_waiting::number_or_left_bracket:
			if (*std::find(numbers.begin(), numbers.end(), infix_str[i]) || infix_str[i] == special_signes[1]) {
				if (*std::find(numbers.begin(), numbers.end(), infix_str[i])) {
					state = number_or_operation_or_point_or_right_bracket;
				}
				if (i == infix_str.size() - 1) {
					return true;
				}
			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number_or_operation_or_point_or_right_bracket:
			if (infix_str[i] == special_signes[0] || infix_str[i] == special_signes[2] || *std::find(numbers.begin(), numbers.end(), infix_str[i]) || *std::find(operations.begin(), operations.end(), infix_str[i])) {

				if (infix_str[i] == special_signes[0]) {
					state = states_of_waiting::number;
				}
				else if (infix_str[i] == special_signes[2]) {
					state = states_of_waiting::operation_or_right_bracket;
					if (i == infix_str.size() - 1) {
						return true;
					}
				}
				else if (*std::find(operations.begin(), operations.end(), infix_str[i])) {
					state = states_of_waiting::number_or_left_bracket;
				}
				else if (*std::find(numbers.begin(), operations.end(), infix_str[i])) {
					if (i == infix_str.size() - 1) {
						return true;
					}
				}

			}
			else {
				return false;
			}

		case states_of_waiting::number:
			if (*std::find(numbers.begin(), numbers.end(), infix_str[i])) {
				state = states_of_waiting::number_or_operation_or_point_or_right_bracket;
				if (i == infix_str.size() - 1) {
					return true;
				}
			}
			else {
				return false;
			}

		case operation_or_right_bracket:
			if (*std::find(numbers.begin(), numbers.end(), infix_str[i]) || *std::find(operations.begin(), operations.end(), infix_str[i])) {

			}
			else {
				return false;
			}

		}
	}

	return false;
}

double expression::calculate() {

}



