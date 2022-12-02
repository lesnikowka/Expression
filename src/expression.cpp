#include "expression.h"

expression::expression(std::string str) : infix_str(str) {}
expression::expression(const expression& ex) : infix_str(ex.infix_str),postfix_str(ex.postfix_str), infix(ex.infix), postfix(ex.postfix) {}
expression::expression(std::string str, std::initializer_list<std::pair<std::string, double>> list) : expression(str) {
	for (auto i : list) {
		if (variables.find(i.first) != variables.end()) 
			throw std::exception("you can't change constants");
		
		variables.insert(i);
	}
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
		if (second == 0)
			throw std::exception("division by zero");
		return first / second;
	}
	return 0;
}

std::string expression::get_infix() { 
	return infix_str; 
}
std::string expression::get_postfix() {
	return postfix_str;
}


bool expression::is_in_vector(const std::vector<char>& v, char value) {
	return std::find(v.begin(), v.end(), value) != v.end();
}

bool expression::check_brackets() {
	std::stack<char> st;
	std::string left_brackets = "([{";
	std::string right_brackets = ")]}";

	for (char element : infix_str) {
		if (left_brackets.find(element) != -1 || right_brackets.find(element) != -1) {
			if (left_brackets.find(element) != -1)
				st.push(right_brackets[left_brackets.find(element)]);
			else if (!st.empty() && st.top() == element)
				st.pop();
			else
				return false;
		}
	}
	return st.empty();
}

bool expression::split() {
	states_of_waiting state = states_of_waiting::number_or_left_bracket_or_unary_minus_or_symbol;
	std::vector<std::pair<std::string, type_of_literal>> tmp_split;

	bool unaryMinusIsPreviousLiteral = false;

	size_t start = 0;

	if (!check_brackets()) {
		return false;
	}

	for (size_t i = 0; i < infix_str.size(); i++) {
		switch (state) {

		case states_of_waiting::symbol_or_operation_or_right_bracket:
			if(is_in_vector(symbols, infix_str[i]) || is_in_vector(operations, infix_str[i])||is_in_vector(right_brackets,infix_str[i])){
				if (is_in_vector(symbols, infix_str[i])) {
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
				}
				if (is_in_vector(operations, infix_str[i])) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start), type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i, 1), type_of_literal::operation));
					start = i + 1;
				}
				if (is_in_vector(right_brackets, infix_str[i])) {
					state = states_of_waiting::operation_or_right_bracket;
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start), type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i, 1), type_of_literal::right_bracket));
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
				}
			}
			else {
				return false;
			}
			break;
			

		case states_of_waiting::number_or_left_bracket_or_unary_minus_or_symbol:
			unaryMinusIsPreviousLiteral = false;
			if (is_in_vector(numbers, infix_str[i]) || is_in_vector(left_brackets, infix_str[i]) || infix_str[i] == (char)special_signes::unary_minus|| is_in_vector(symbols, infix_str[i])) {
				if (is_in_vector(symbols, infix_str[i])) {
					state = states_of_waiting::symbol_or_operation_or_right_bracket;
				}
				else if (is_in_vector(numbers, infix_str[i])) {
					state = states_of_waiting::number_or_operation_or_point_or_right_bracket;
				}
				else if (is_in_vector(left_brackets, infix_str[i])) {
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1),type_of_literal::left_bracket));
					start = i + 1;
				}
				else if (infix_str[i] == (char)special_signes::unary_minus) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
					start = i;
					unaryMinusIsPreviousLiteral = true;
				}
				if (i == infix_str.size() - 1 && infix_str[i] != (char)special_signes::unary_minus) {
					state = states_of_waiting::success;
				}
			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number_or_left_bracket_or_symbol:
			if (is_in_vector(numbers, infix_str[i]) || is_in_vector(left_brackets, infix_str[i]) || is_in_vector(symbols, infix_str[i])) {
				if (is_in_vector(symbols, infix_str[i])) {
					state = states_of_waiting::symbol_or_operation_or_right_bracket;
				}
				else if (is_in_vector(numbers, infix_str[i])) {
					state = states_of_waiting::number_or_operation_or_point_or_right_bracket;
				}
				else if (is_in_vector(left_brackets, infix_str[i])) {
					state = states_of_waiting::number_or_left_bracket_or_unary_minus_or_symbol;
					if (unaryMinusIsPreviousLiteral) {
						tmp_split.push_back(std::make_pair("-1", type_of_literal::operand));
						tmp_split.push_back(std::make_pair("*", type_of_literal::operation));
					}
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1), type_of_literal::left_bracket));
					start = i + 1;
				}
				if (i == infix_str.size() - 1) {
					state = states_of_waiting::success;
				}
				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number_or_operation_or_right_bracket:
			if (is_in_vector(right_brackets, infix_str[i]) || is_in_vector(numbers, infix_str[i]) || is_in_vector(operations, infix_str[i])) {

				if (is_in_vector(right_brackets, infix_str[i])) {
					state = states_of_waiting::operation_or_right_bracket;
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start),type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1),type_of_literal::right_bracket));
				}
				else if (is_in_vector(operations, infix_str[i])) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start), type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i,1), type_of_literal::operation));
					start = i + 1;
				}
				else if (is_in_vector(numbers, infix_str[i])) {
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
				}
				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number_or_operation_or_point_or_right_bracket:
			if (infix_str[i] == (char)special_signes::point || is_in_vector(right_brackets, infix_str[i]) || is_in_vector(numbers, infix_str[i]) || is_in_vector(operations, infix_str[i])) {

				if (infix_str[i] == (char)special_signes::point) {
					state = states_of_waiting::number;
				}
				else if (is_in_vector(right_brackets, infix_str[i])) {
					state = states_of_waiting::operation_or_right_bracket;
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start), type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i, 1), type_of_literal::right_bracket));
				}
				else if (is_in_vector(operations, infix_str[i])) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, i - start), type_of_literal::operand));
					tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(i, 1), type_of_literal::operation));
					start = i + 1;
				}
				else if (is_in_vector(numbers, infix_str[i])) {
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
				}
				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number:
			if (is_in_vector(numbers, infix_str[i])) {
				state = states_of_waiting::number_or_operation_or_right_bracket;
				if (i == infix_str.size() - 1) {
					state = states_of_waiting::success;
				}
				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;

		case states_of_waiting::operation_or_right_bracket:
			if (is_in_vector(right_brackets, infix_str[i]) || is_in_vector(operations, infix_str[i])) {
				if (is_in_vector(operations, infix_str[i])) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
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
				unaryMinusIsPreviousLiteral = false;
			}
			else {
				return false;
			}
			break;
		}
	}


	if(state == states_of_waiting::success){
		if (is_in_vector(numbers, infix_str[infix_str.size()-1]) || is_in_vector(symbols, infix_str[infix_str.size() - 1])) {
			tmp_split.push_back(std::pair<std::string, type_of_literal>(infix_str.substr(start, infix_str.size() - start),type_of_literal::operand));
		};

		//for (auto i : tmp_split) {
		//	std::cout << "\"" << i.first << "\",   ";
		//}
		//std::cout << std::endl;

		infix = tmp_split;
		return true;
	}

	return false;
}

double expression::calculate() {

	if (split()) {
		to_postfix();
		
		std::stack<double> values;
		
		double first_op, second_op;

		for (auto i : postfix) {
			if (i.second == type_of_literal::operand)
				values.push(request_variables(i.first));
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
	else {
		std::cout << "expression is not correct" << std::endl;
		return 0;
	}
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
			while (!stack.empty() && stack.top().second == type_of_literal::operation && (*priorities.find(literal.first[0])).second <= (*priorities.find(stack.top().first[0])).second) {
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
	for (auto& literal : postfix)
		postfix_str += literal.first;
}

double expression::request_variables(std::string var) {
	if (!is_in_vector(symbols, var.back()))
		return std::stod(var);

	double value;
	if (var.front() == (char)special_signes::unary_minus) {
		if (variables.find(var.substr(1)) == variables.end()) {
			std::cout << var.substr(1) << ": ";
			std::cin >> value;
			std::cout << std::endl;
			variables.emplace(var.substr(1), -value);
		}
		else 
			value = -variables[var.substr(1)];
	}
	else{
		if (variables.find(var) == variables.end()) {
			std::cout << var << ": ";
			std::cin >> value;
			std::cout << std::endl;
			variables.emplace(var, value);
		}
		else
			value = variables[var];
	}
	return value;
}

