#include "expression.h"



void expression::change_expression(std::string ex) {
	clear();

	infix_str = ex;

	if (!split()) {
		throw "incorrect input";
	}

	to_postfix();
}

bool expression::correct_name(std::string var) {
	for (auto i : var) {
		if (!is_in_vector(symbols, i)) {
			return false;
		}
	}

	return true;
}

bool expression::check_brackets() {
	std::stack<char> st;
	std::string left_brackets = "([{";
	std::string right_brackets = ")]}";

	for (char element : infix_str) {
		if (left_brackets.find(element) != -1 || right_brackets.find(element) != -1) {
			if (left_brackets.find(element) != -1) {
				st.push(right_brackets[left_brackets.find(element)]);
			}

			else if (!st.empty() && st.top() == element) {
				st.pop();
			}

			else {
				return false;
			}
		}
	}

	return st.empty();
}

bool expression::split() {
	waiting state = waiting::number_or_left_bracket_or_unary_minus_or_symbol;

	std::vector<std::pair<std::string, type>> tmp_split;

	bool unaryMinusIsPreviousLiteral = false;

	size_t start = 0;

	if (!check_brackets()) {
		return false;
	}

	for (size_t i = 0; i < infix_str.size(); i++) {
		char symbol = infix_str[i];
		switch (state) {

		case waiting::symbol_or_operation_or_right_bracket:
			if (is_symbol(symbol) || is_operation(symbol) || is_right_bracket(symbol)) {
				if (is_symbol(symbol)) {
					if (i == infix_str.size() - 1) {
						state = waiting::success;
					}
				}

				if (is_operation(symbol)) {
					state = waiting::number_or_left_bracket_or_symbol;

					tmp_split.push_back({ infix_str.substr(start, i - start), type::operand });
					tmp_split.push_back({ infix_str.substr(i, 1), type::operation });

					start = i + 1;
				}

				if (is_right_bracket(symbol)) {
					state = waiting::operation_or_right_bracket;

					tmp_split.push_back({ infix_str.substr(start, i - start), type::operand });
					tmp_split.push_back({ infix_str.substr(i, 1), type::right_bracket });

					if (i == infix_str.size() - 1) {
						state = waiting::success;
					}
				}
			}
			else {
				return false;
			}
			break;


		case waiting::number_or_left_bracket_or_unary_minus_or_symbol:
			unaryMinusIsPreviousLiteral = false;
			if (is_in_vector(numbers, symbol) || is_left_bracket(symbol) || symbol == '-' || is_symbol(symbol)) {
				if (is_symbol(symbol)) {
					state = waiting::symbol_or_operation_or_right_bracket;
				}

				else if (is_in_vector(numbers, symbol)) {
					state = waiting::number_or_operation_or_point_or_right_bracket;
				}

				else if (is_left_bracket(symbol)) {
					tmp_split.push_back({ infix_str.substr(i,1),type::left_bracket });
					start = i + 1;
				}

				else if (symbol == '-') {
					state = waiting::number_or_left_bracket_or_symbol;
					start = i;
					unaryMinusIsPreviousLiteral = true;
				}

				if (i == infix_str.size() - 1 && symbol != '-') {
					state = waiting::success;
				}
			}
			else {
				return false;
			}
			break;

		case waiting::number_or_left_bracket_or_symbol:
			if (is_in_vector(numbers, symbol) || is_left_bracket(symbol) || is_symbol(symbol)) {
				if (is_symbol(symbol)) {
					state = waiting::symbol_or_operation_or_right_bracket;
				}

				else if (is_in_vector(numbers, symbol)) {
					state = waiting::number_or_operation_or_point_or_right_bracket;
				}

				else if (is_left_bracket(symbol)) {
					state = waiting::number_or_left_bracket_or_unary_minus_or_symbol;

					if (unaryMinusIsPreviousLiteral) {
						tmp_split.push_back(std::make_pair("-1", type::operand));
						tmp_split.push_back(std::make_pair("*", type::operation));
					}

					tmp_split.push_back({ infix_str.substr(i,1), type::left_bracket });

					start = i + 1;
				}
				if (i == infix_str.size() - 1) {
					state = waiting::success;
				}

				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;

		case waiting::number_or_operation_or_right_bracket:
			if (is_right_bracket(symbol) || is_in_vector(numbers, symbol) || is_operation(symbol)) {

				if (is_right_bracket(symbol)) {
					state = waiting::operation_or_right_bracket;

					if (i == infix_str.size() - 1) {
						state = waiting::success;
					}

					tmp_split.push_back({ infix_str.substr(start, i - start),type::operand });
					tmp_split.push_back({ infix_str.substr(i,1),type::right_bracket });
				}

				else if (is_operation(symbol)) {
					state = waiting::number_or_left_bracket_or_symbol;

					tmp_split.push_back({ infix_str.substr(start, i - start), type::operand });
					tmp_split.push_back({ infix_str.substr(i,1), type::operation });

					start = i + 1;
				}

				else if (is_in_vector(numbers, symbol)) {
					if (i == infix_str.size() - 1) {
						state = waiting::success;
					}
				}

				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;

		case waiting::number_or_operation_or_point_or_right_bracket:
			if (symbol == '.' || is_right_bracket(symbol) || is_in_vector(numbers, symbol) || is_operation(symbol)) {

				if (symbol == '.') {
					state = waiting::number;
				}

				else if (is_right_bracket(symbol)) {
					state = waiting::operation_or_right_bracket;

					if (i == infix_str.size() - 1) {
						state = waiting::success;
					}

					tmp_split.push_back({ infix_str.substr(start, i - start), type::operand });
					tmp_split.push_back({ infix_str.substr(i, 1), type::right_bracket });
				}

				else if (is_operation(symbol)) {
					state = waiting::number_or_left_bracket_or_symbol;

					tmp_split.push_back({ infix_str.substr(start, i - start), type::operand });
					tmp_split.push_back({ infix_str.substr(i, 1), type::operation });

					start = i + 1;
				}

				else if (is_in_vector(numbers, symbol)) {
					if (i == infix_str.size() - 1) {
						state = waiting::success;
					}
				}

				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;

		case waiting::number:
			if (is_in_vector(numbers, symbol)) {
				state = waiting::number_or_operation_or_right_bracket;

				if (i == infix_str.size() - 1) {
					state = waiting::success;
				}

				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;

		case waiting::operation_or_right_bracket:
			if (is_right_bracket(symbol) || is_operation(symbol)) {

				if (is_operation(symbol)) {
					state = waiting::number_or_left_bracket_or_symbol;

					if (i == infix_str.size() - 1) {
						return false;
					}

					tmp_split.push_back({ infix_str.substr(i,1),type::operation });

					start = i + 1;
				}

				else {
					if (i == infix_str.size() - 1) {
						state = waiting::success;
					}

					start = i + 1;

					tmp_split.push_back({ infix_str.substr(i,1),type::right_bracket });
				}

				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;
		}
	}


	if (state == waiting::success) {
		if (is_number(infix_str[infix_str.size() - 1]) || is_symbol(infix_str[infix_str.size() - 1])) {
			tmp_split.push_back({ infix_str.substr(start, infix_str.size() - start), type::operand });
		}

		infix = tmp_split;

		return true;
	}

	return false;
}
