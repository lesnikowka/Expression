#include "expression.h"

expression::expression(std::string str) : infix_str(str) {
	if (!split()) 
		throw "incorrect input";
	to_postfix();

}

expression::expression(std::string str, std::initializer_list<std::pair<std::string, double>> list) : expression(str) {
	for (auto i : list) {
		add_variable(i);
		
		variables.insert(i);
	}
}

bool expression::is_number(char value) {
	return is_in_vector(numbers, value);
}
bool expression::is_symbol(char value) {
	return is_in_vector(symbols, value);
}
bool expression::is_right_bracket(char value) {
	return is_in_vector(right_brackets, value);
}
bool expression::is_left_bracket(char value) {
	return is_in_vector(left_brackets, value);
}
bool expression::is_operation(char value) {
	return is_in_vector(operations, value);
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
			throw "division by zero";
		return first / second;
	}
	return 0;
}

void expression::add_variable(std::pair<std::string, double> var){
	if (!correct_name(var.first)) {
		throw "incorrect name of variable";
	}
	else if (variables.find(var.first) != variables.end()) {
		throw "variable was added earlier";
	}
	else {
		variables.insert(var);
	}
}

void expression::clear() {
	variables = constants;
	infix_str.clear();
	postfix_str.clear();
	infix.clear();
	postfix.clear();
}

void expression::change_expression(std::string ex) {
	clear();
	infix_str = ex;
	if (!split())
		throw "incorrect input";
	to_postfix();
}

bool expression::correct_name(std::string var) {
	for (auto i : var)
		if (!is_in_vector(symbols, i))
			return false;
	return true;
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
		char symbol = infix_str[i];
		switch (state) {

		case states_of_waiting::symbol_or_operation_or_right_bracket:
			if(is_symbol(symbol) || is_operation(symbol)|| is_right_bracket(symbol)){
				if (is_symbol(symbol)) {
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
				}
				if (is_operation(symbol)) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
					tmp_split.push_back({ infix_str.substr(start, i - start), type_of_literal::operand });
					tmp_split.push_back({infix_str.substr(i, 1), type_of_literal::operation});
					start = i + 1;
				}
				if (is_right_bracket(symbol)) {
					state = states_of_waiting::operation_or_right_bracket;
					tmp_split.push_back({infix_str.substr(start, i - start), type_of_literal::operand});
					tmp_split.push_back({infix_str.substr(i, 1), type_of_literal::right_bracket});
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
			if (is_in_vector(numbers, symbol) || is_left_bracket(symbol) || symbol == (char)special_signes::unary_minus|| is_symbol(symbol)) {
				if (is_symbol(symbol)) {
					state = states_of_waiting::symbol_or_operation_or_right_bracket;
				}
				else if (is_in_vector(numbers, symbol)) {
					state = states_of_waiting::number_or_operation_or_point_or_right_bracket;
				}
				else if (is_left_bracket(symbol)) {
					tmp_split.push_back({infix_str.substr(i,1),type_of_literal::left_bracket});
					start = i + 1;
				}
				else if (symbol == (char)special_signes::unary_minus) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
					start = i;
					unaryMinusIsPreviousLiteral = true;
				}
				if (i == infix_str.size() - 1 && symbol != (char)special_signes::unary_minus) {
					state = states_of_waiting::success;
				}
			}
			else {
				return false;
			}
			break;

		case states_of_waiting::number_or_left_bracket_or_symbol:
			if (is_in_vector(numbers, symbol) || is_left_bracket(symbol) || is_symbol(symbol)) {
				if (is_symbol(symbol)) {
					state = states_of_waiting::symbol_or_operation_or_right_bracket;
				}
				else if (is_in_vector(numbers, symbol)) {
					state = states_of_waiting::number_or_operation_or_point_or_right_bracket;
				}
				else if (is_left_bracket(symbol)) {
					state = states_of_waiting::number_or_left_bracket_or_unary_minus_or_symbol;
					if (unaryMinusIsPreviousLiteral) {
						tmp_split.push_back(std::make_pair("-1", type_of_literal::operand));
						tmp_split.push_back(std::make_pair("*", type_of_literal::operation));
					}
					tmp_split.push_back({infix_str.substr(i,1), type_of_literal::left_bracket});
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
			if (is_right_bracket(symbol) || is_in_vector(numbers, symbol) || is_operation(symbol)) {

				if (is_right_bracket(symbol)) {
					state = states_of_waiting::operation_or_right_bracket;
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
					tmp_split.push_back({infix_str.substr(start, i - start),type_of_literal::operand});
					tmp_split.push_back({infix_str.substr(i,1),type_of_literal::right_bracket});
				}
				else if (is_operation(symbol)) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
					tmp_split.push_back({infix_str.substr(start, i - start), type_of_literal::operand});
					tmp_split.push_back({infix_str.substr(i,1), type_of_literal::operation});
					start = i + 1;
				}
				else if (is_in_vector(numbers, symbol)) {
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
			if (symbol == (char)special_signes::point || is_right_bracket(symbol) || is_in_vector(numbers, symbol) || is_operation(symbol)) {

				if (symbol == (char)special_signes::point) {
					state = states_of_waiting::number;
				}
				else if (is_right_bracket(symbol)) {
					state = states_of_waiting::operation_or_right_bracket;
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
					tmp_split.push_back({infix_str.substr(start, i - start), type_of_literal::operand});
					tmp_split.push_back({infix_str.substr(i, 1), type_of_literal::right_bracket});
				}
				else if (is_operation(symbol)) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
					tmp_split.push_back({infix_str.substr(start, i - start), type_of_literal::operand});
					tmp_split.push_back({infix_str.substr(i, 1), type_of_literal::operation});
					start = i + 1;
				}
				else if (is_in_vector(numbers, symbol)) {
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
			if (is_in_vector(numbers, symbol)) {
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
			if (is_right_bracket(symbol) || is_operation(symbol)) {
				if (is_operation(symbol)) {
					state = states_of_waiting::number_or_left_bracket_or_symbol;
					if (i == infix_str.size() - 1) {
						return false;
					}
					tmp_split.push_back({infix_str.substr(i,1),type_of_literal::operation});
					start = i + 1;
				}
				else {
					if (i == infix_str.size() - 1) {
						state = states_of_waiting::success;
					}
					start = i + 1;
					tmp_split.push_back({infix_str.substr(i,1),type_of_literal::right_bracket});
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
			tmp_split.push_back({infix_str.substr(start, infix_str.size() - start),type_of_literal::operand});
		};

		infix = tmp_split;
		return true;
	}

	return false;
}

double expression::calculate() {
	std::stack<double> values;
	
	double first_op, second_op;

	for (auto i : postfix) {
		std::string literal = i.first;
		if (i.second == type_of_literal::operand) {
			if (is_in_vector(symbols, i.first.back())) {
				if (i.first.front() == (char)special_signes::unary_minus)
					literal = literal.substr(1);
				if (variables.find(literal) == variables.end()) throw "variable was not input";
				values.push(variables[literal] * (1 - 2 * (int)(i.first.front() == (char)special_signes::unary_minus)));
			}
			else {
				values.push(std::stod(literal));
			}
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
	std::stack<std::pair<std::string, type_of_literal>> stack;

	for (const auto& literal : infix) {
		if (literal.second == type_of_literal::operand) {
			postfix.push_back({literal.first, literal.second});
		}
		else if (literal.second == type_of_literal::left_bracket) {
			stack.push(std::pair<std::string, type_of_literal>(literal.first, literal.second));
		}
		else if (literal.second == type_of_literal::right_bracket) {
			while (!stack.empty() && stack.top().second != type_of_literal::left_bracket) {
				postfix.push_back({stack.top().first, stack.top().second});
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
	for (auto literal : postfix)
		postfix_str += literal.first;
}

void expression::request_variables() {
	double value;

	for (auto var : infix) {
		if (is_in_vector(symbols, var.first.back())) {
			if (var.first.front() == (char)special_signes::unary_minus && variables.find(var.first.substr(1)) == variables.end()) {
				std::cout << var.first.substr(1) << " = ";
				std::cin >> value;
				std::cout << std::endl;
				add_variable(std::make_pair(var.first.substr(1),-value));
			}

			else if(variables.find(var.first) == variables.end()){
				std::cout << var.first << " = ";
				std::cin >> value;
				std::cout << std::endl;
				add_variable(std::make_pair(var.first, value));
			}
		}
	}
}

