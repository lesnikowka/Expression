#include "expression.h"



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
			throw "division by zero";
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
			if (is_in_vector(symbols, i.first.back())) {
				if (i.first.front() == '-') {
					literal = literal.substr(1);
				}

				if (variables.find(literal) == variables.end()) {
					throw "variable was not input";
				}

				values.push(variables[literal] * (1 - 2 * (int)(i.first.front() == '-')));
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