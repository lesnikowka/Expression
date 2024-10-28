#include "expression.h"

expression::expression(std::string str) : infix_str(str) {
	if (!split()) {
		throw std::invalid_argument("incorrect input");
	}

	to_postfix();
}

std::string expression::get_infix() { 
	return infix_str; 
}

std::string expression::get_postfix() { 
	return postfix_str; 
}

void expression::clear() {
	infix_str.clear();
	postfix_str.clear();
	infix.clear();
	postfix.clear();
}

double expression::operate(double first, double second, char operation) {
	switch (operation) {
	case '+':
		return first + second;
	case '-':
		return first - second;
	case '*':
		return first * second;
	case '/':
		if (second == 0) {
			throw std::invalid_argument("Division by zero");
		}
		return first / second;
	}
	return 0;
}

double expression::calculate() {
	std::stack<double> values;

	double first_op, second_op;

	for (auto i : postfix) {
		std::string literal = i.first;

		if (i.second == type::operand) {
			values.push(std::stod(literal));
		}

		else {
			second_op = values.top();
			values.pop();
			first_op = values.top();
			values.pop();

			values.push(operate(first_op, second_op, i.first.front()));
		}
	}

	return values.top();
}

void expression::to_postfix() {
	std::stack<std::pair<std::string, type>> stack;

	for (const auto& literal : infix) {
		if (literal.second == type::operand) {
			postfix.push_back({ literal.first, literal.second });
		}

		else if (literal.second == type::left_bracket) {
			stack.push(std::pair<std::string, type>(literal.first, literal.second));
		}

		else if (literal.second == type::right_bracket) {
			while (!stack.empty() && stack.top().second != type::left_bracket) {
				postfix.push_back({ stack.top().first, stack.top().second });
				stack.pop();
			}

			stack.pop();
		}

		else if (literal.second == type::operation) {
			while (!stack.empty() && stack.top().second == type::operation && (*priorities.find(literal.first[0])).second <= (*priorities.find(stack.top().first[0])).second) {
				postfix.push_back(stack.top());
				stack.pop();
			}

			stack.push(literal);
		}
	}

	while (!stack.empty()) {
		postfix.push_back(stack.top());

		stack.pop();
	}

	for (auto literal : postfix) {
		postfix_str += literal.first;
	}
}

bool expression::is_in_vector(const std::vector<char>& v, char value) {
	return std::find(v.begin(), v.end(), value) != v.end();
}

bool expression::is_number(char value) {
	return is_in_vector(numbers, value);
}

bool expression::is_right_bracket(char value) {
	return value == ')';
}

bool expression::is_left_bracket(char value) {
	return value == '(';
}

bool expression::is_operation(char value) {
	return is_in_vector(operations, value);
}

std::istream& operator>>(std::istream& in, expression& ex) {
	ex.clear();

	in >> ex.infix_str;

	if (!ex.split()) {
		throw std::invalid_argument("incorrect input");
	}

	ex.to_postfix();

	return in;
}

void expression::change_expression(std::string ex) {
	clear();

	infix_str = ex;

	if (!split()) {
		throw std::invalid_argument("incorrect input");
	}

	to_postfix();
}

bool expression::check_brackets() {
	std::stack<char> st;

	for (char element : infix_str) {
		if (is_left_bracket(element) || is_right_bracket(element)) {
			if (is_left_bracket(element)) {
				st.push(')');
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
	waiting state = waiting::number_or_left_bracket_or_unary_minus;

	std::vector<std::pair<std::string, type>> tmp_split;

	bool unaryMinusIsPreviousLiteral = false;

	size_t start = 0;

	if (!check_brackets()) {
		return false;
	}

	for (size_t i = 0; i < infix_str.size(); i++) {
		char symbol = infix_str[i];
		switch (state) {
		case waiting::number_or_left_bracket_or_unary_minus:
			unaryMinusIsPreviousLiteral = false;
			if (is_number(symbol) || is_left_bracket(symbol) || symbol == '-') {
				if (is_number(symbol)) {
					state = waiting::number_or_operation_or_point_or_right_bracket;
				}

				else if (is_left_bracket(symbol)) {
					tmp_split.push_back({ infix_str.substr(i,1),type::left_bracket });
					start = i + 1;
				}

				else if (symbol == '-') {
					state = waiting::number_or_left_bracket;
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

		case waiting::number_or_left_bracket:
			if (is_number(symbol) || is_left_bracket(symbol)) {
				if (is_number(symbol)) {
					state = waiting::number_or_operation_or_point_or_right_bracket;
				}

				else if (is_left_bracket(symbol)) {
					state = waiting::number_or_left_bracket_or_unary_minus;

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
			if (is_right_bracket(symbol) || is_number(symbol) || is_operation(symbol)) {

				if (is_right_bracket(symbol)) {
					state = waiting::operation_or_right_bracket;

					if (i == infix_str.size() - 1) {
						state = waiting::success;
					}

					tmp_split.push_back({ infix_str.substr(start, i - start),type::operand });
					tmp_split.push_back({ infix_str.substr(i,1),type::right_bracket });
				}

				else if (is_operation(symbol)) {
					state = waiting::number_or_left_bracket;

					tmp_split.push_back({ infix_str.substr(start, i - start), type::operand });
					tmp_split.push_back({ infix_str.substr(i,1), type::operation });

					start = i + 1;
				}

				else if (is_number(symbol)) {
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
			if (symbol == '.' || is_right_bracket(symbol) || is_number(symbol) || is_operation(symbol)) {

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
					state = waiting::number_or_left_bracket;

					tmp_split.push_back({ infix_str.substr(start, i - start), type::operand });
					tmp_split.push_back({ infix_str.substr(i, 1), type::operation });

					start = i + 1;
				}

				else if (is_number(symbol)) {
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
			if (is_number(symbol)) {
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
					state = waiting::number_or_left_bracket;

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
		if (is_number(infix_str[infix_str.size() - 1])) {
			tmp_split.push_back({ infix_str.substr(start, infix_str.size() - start), type::operand });
		}

		infix = tmp_split;

		return true;
	}

	return false;
}
