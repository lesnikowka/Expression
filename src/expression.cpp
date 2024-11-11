#include "expression.h"

template<class T>
T operateT(T first, T second, char operation)
{
	switch (operation) {
	case '+':
		return first + second;
	case '-':
		return first - second;
	case '*':
		return first * second;
	case '/':
		if (second == 0) {
			throw std::invalid_argument("division by zero");
		}
		return first / second;
	}
	return 0;
}

expression::expression(std::string str) : infix_str(str) {
	if (!split()) {
		throw std::invalid_argument( "incorrect input");
	}

	to_postfix();
}

expression::expression(std::string str, std::initializer_list<std::pair<std::string, double>> list) : expression(str) {
	for (auto i : list) {
		add_variable(i);
		
		variables.insert({ i.first, {i.second, type::operand_double}});
	}
}

expression::expression(std::string str, std::initializer_list<std::pair<std::string, int>> list) {
	for (auto i : list) {
		add_variable(i);

		variables.insert({ i.first, {(double)i.second, type::operand_int} });
	}
}

std::string expression::get_infix() { 
	return infix_str; 
}

std::string expression::get_postfix() { 
	return postfix_str; 
}

void expression::check_variable_name(const std::string& name)
{
	if (!correct_name(name)) {
		throw std::invalid_argument("incorrect name of variable");
	}

	else if (variables.find(name) != variables.end()) {
		throw std::invalid_argument("variable was added earlier");
	}
}

void expression::add_variable(std::pair<std::string, double> var){
	check_variable_name(var.first);
	variables.insert({ var.first, {var.second, type::operand_double} });
}

void expression::add_variable(std::pair<std::string, int> var) {
	check_variable_name(var.first);
	variables.insert({var.first, {(double)var.second, type::operand_int} });
}

void expression::clear() {
	infix_str.clear();
	postfix_str.clear();
	infix.clear();
	postfix.clear();
}

std::pair<double, expression::type> expression::operate(std::pair<double, type> first, std::pair<double, type> second, char operation){
	if ((first.second == type::operand_double || second.second == type::operand_double) && operation == '%') {
		throw std::invalid_argument("Mod is not allowed for double");
	}

	if (first.second == type::operand_int && second.second == type::operand_int) {
		if (operation == '%')
		{
			return std::make_pair((double)((int)first.first % (int)second.first), type::operand_int);
		}
		return std::make_pair((double)operateT<int>(first.first, second.first, operation), type::operand_int);
	}

	return std::make_pair(operateT<double>(first.first, second.first, operation), type::operand_double);
}

double expression::calculate() {
	std::stack<std::pair<double, type>> values;

	for (auto i : postfix) {
		std::string literal = i.first;

		if (i.second == type::operand_int || i.second == type::operand_double || i.second == type::operand) {
			if (is_in_vector(symbols, i.first.back())) {
				if (i.first.front() == '-') {
					literal = literal.substr(1);
				}

				if (variables.find(literal) == variables.end()) {
					throw std::invalid_argument( "variable was not input");
				}

				values.push({ variables[literal].first * (1 - 2 * (int)(i.first.front() == '-')),
					variables[literal].second });
			}
			else {
				type literalType = literal.find(".") != -1 ? type::operand_double : type::operand_int;
				values.push({ std::stod(literal), literalType });
			}
		}

		else {
			auto second_op = values.top();
			values.pop();
			auto first_op = values.top();
			values.pop();

			values.push(operate(first_op, second_op, i.first.front()));
		}
	}

	return values.top().first;
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

bool expression::is_symbol(char value) {
	return is_in_vector(symbols, value);
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

void expression::request_variables() {
	std::string value;

	for (auto var : infix) {
		if (is_in_vector(symbols, var.first.back())) {
			if (var.first.front() == '-' && variables.find(var.first.substr(1)) == variables.end()) {
				std::cout << var.first.substr(1) << " = ";
				std::cin >> value;
				std::cout << std::endl;

				if (value.find('.') != -1) {
					add_variable(std::make_pair(var.first.substr(1), -std::stod(value)));
				}
				else {
					add_variable(std::make_pair(var.first.substr(1), -std::stoi(value)));
				}
			}

			else if (variables.find(var.first) == variables.end()) {
				std::cout << var.first << " = ";
				std::cin >> value;
				std::cout << std::endl;

				if (value.find('.') != -1) {
					add_variable(std::make_pair(var.first, std::stod(value)));
				}
				else {
					add_variable(std::make_pair(var.first, std::stoi(value)));
				}
			}
		}
	}
}

std::istream& operator>>(std::istream& in, expression& ex) {
	ex.clear();

	in >> ex.infix_str;

	if (!ex.split()) {
		throw std::invalid_argument( "incorrect input");
	}

	ex.to_postfix();
	ex.request_variables();

	return in;
}

void expression::change_expression(std::string ex) {
	clear();

	infix_str = ex;

	if (!split()) {
		throw std::invalid_argument( "incorrect input");
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
