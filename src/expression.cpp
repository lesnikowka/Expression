#include "expression.h"

expression::expression(std::string str) : infix_str(str) {}
expression::expression(const expression& ex) : infix_str(ex.infix_str), infix(ex.infix), postfix(ex.postfix) {}
int expression::operation_location(char element) {
	for (int i = 0; i < operations.size(); i++) {
		if (operations[i] == element) {
			return i;
		}
	}
	return -1;
}

std::string expression::get_infix() { 
	return infix_str; 
}
std::string expression::get_postfix() {
	std::string postfix_str;
	for (auto& literal : postfix) {
		postfix_str += literal.first;
	}
	return postfix_str;
}

bool expression::is_in_vector(std::vector<char>::iterator i1, std::vector<char>::iterator i2, char val) {
	while (i1 != i2) {
		if (*i1 == val) {
			return true;
		}
		i1++;
	}
	return false;
}

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
	states_of_waiting state = states_of_waiting::number_or_left_bracket_or_unary_minus;
	std::vector<std::pair<std::string, type_of_literal>> tmp_split;

	size_t start = 0;

	if (!check_brackets()) {
		return false;
	}

	for (size_t i = 0; i < infix_str.size(); i++) {
		switch (state) {

		case states_of_waiting::number_or_left_bracket_or_unary_minus:
			if (is_in_vector(numbers.begin(), numbers.end(), infix_str[i]) || infix_str[i] == special_signes[1] || infix_str[i] == special_signes[3]) {
				if (is_in_vector(numbers.begin(), numbers.end(), infix_str[i])) {
					state = states_of_waiting::number_or_operation_or_point_or_right_bracket;
				}
				else if (infix_str[i] == special_signes[1]) {
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1),type_of_literal::left_bracket));
					start = i + 1;
				}
				else if (infix_str[i] == special_signes[3]) {
					state = states_of_waiting::number_or_left_bracket;
					start = i;
				}
				if (i == infix_str.size() - 1 && infix_str[i] != special_signes[3]) {
					state = states_of_waiting::success;
				}
			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number_or_left_bracket:
			if (is_in_vector(numbers.begin(), numbers.end(), infix_str[i]) || infix_str[i] == special_signes[1]) {
				if (is_in_vector(numbers.begin(), numbers.end(), infix_str[i])) {
					state = number_or_operation_or_point_or_right_bracket;
				}
				else if (infix_str[i] == special_signes[1]) {
					state = number_or_left_bracket_or_unary_minus;
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1), type_of_literal::left_bracket));
					start = i + 1;
				}
				if (i == infix_str.size() - 1) {
					state = states_of_waiting::success;
				}
			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number_or_operation_or_right_bracket:
			if (infix_str[i] == special_signes[2] || is_in_vector(numbers.begin(), numbers.end(), infix_str[i]) || is_in_vector(operations.begin(), operations.end(), infix_str[i])) {

				if (infix_str[i] == special_signes[2]) {
					state = states_of_waiting::operation_or_right_bracket;
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start),type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1),type_of_literal::right_bracket));
				}
				else if (is_in_vector(operations.begin(), operations.end(), infix_str[i])) {
					state = states_of_waiting::number_or_left_bracket;
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start), type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1), type_of_literal::operation));
					start = i + 1;
				}
				else if (is_in_vector(numbers.begin(), numbers.end(), infix_str[i])) {
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
				}

			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number_or_operation_or_point_or_right_bracket:
			if (infix_str[i] == special_signes[0] || infix_str[i] == special_signes[2] || is_in_vector(numbers.begin(), numbers.end(), infix_str[i]) || is_in_vector(operations.begin(), operations.end(), infix_str[i])) {

				if (infix_str[i] == special_signes[0]) {
					state = states_of_waiting::number;
				}
				else if (infix_str[i] == special_signes[2]) {
					state = states_of_waiting::operation_or_right_bracket;
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start), type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i, 1), type_of_literal::right_bracket));
				}
				else if (is_in_vector(operations.begin(), operations.end(), infix_str[i])) {
					state = states_of_waiting::number_or_left_bracket;
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start), type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i, 1), type_of_literal::operation));
					start = i + 1;
				}
				else if (is_in_vector(numbers.begin(), numbers.end(), infix_str[i])) {
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
				}

			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number:
			if (is_in_vector(numbers.begin(), numbers.end(), infix_str[i])) {
				state = states_of_waiting::number_or_operation_or_right_bracket;
				if (i == infix_str.size() - 1) {
					state = states_of_waiting::success;
				}
			}
			else {
				return false;
			}
			break;

		case operation_or_right_bracket:
			if (infix_str[i] == special_signes[2] || is_in_vector(operations.begin(), operations.end(), infix_str[i])) {
				if (is_in_vector(operations.begin(), operations.end(), infix_str[i])) {
					state = states_of_waiting::number_or_left_bracket;
					if (i == infix_str.size() - 1) {
						return false;
					}
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1),type_of_literal::operation));
					start = i + 1;
				}
				else {
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
					start = i + 1;
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1),type_of_literal::right_bracket));
				}
			}
			else {
				return false;
			}
			break;
		}
	}


	if(state == states_of_waiting::success){
		if (is_in_vector(numbers.begin(), numbers.end(), infix_str[infix_str.size()-1])) {
			tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, infix_str.size() - start),type_of_literal::operand));
		};
		infix = tmp_split;
		return true;
	}

	return false;
}

double expression::calculate() {

	if (split()) std::cout << "ok";
	else std::cout << "expression is incorrect";
	std::cout << std::endl;
	std::cout << std::endl;

	for (auto i : infix) {
		std::cout << i.first << std::endl;
	}

	std::cout << std::endl;

	to_postfix();

	for (auto i : postfix) {
		std::cout << i.first;
	}
	std::cout <<std:: endl;

	return double();
}

void expression::to_postfix() {
	std::stack<std::pair<std::string, type_of_literal>> stack;

	for (const auto& literal : infix) {
		if (literal.second == type_of_literal::operand) {
			postfix.push_back(std::pair<std::string, type_of_literal>(literal.first, literal.second));
		}
		else if (literal.second == type_of_literal::left_bracket) {
			stack.push(std::pair<std::string, type_of_literal>(literal.first, literal.second));
		}
		else if (literal.second == type_of_literal::right_bracket) {
			while (!stack.empty() && stack.top().second != type_of_literal::left_bracket) {
				postfix.push_back(std::pair<std::string, type_of_literal>(stack.top().first, stack.top().second));
				stack.pop();
			}
			stack.pop();
		}
		else if (literal.second == type_of_literal::operation){
			while (!stack.empty() && stack.top().second == type_of_literal::operation && priorities[operation_location(literal.first[0])] <= priorities[operation_location(stack.top().first[0])]) {
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
}



