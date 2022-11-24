#pragma once

#include <string>
#include <vector>
#include <stack>
#include <iostream>

class expression {
	enum states_of_waiting {
		number,
		number_or_left_bracket,
		number_or_left_bracket_or_unary_minus,
		number_or_operation_or_point_or_right_bracket,
		number_or_operation_or_right_bracket,
		operation_or_right_bracket,
		success
	};
	enum type_of_literal {
		operand,
		right_bracket,
		left_bracket,
		operation
	};

	std::string infix_str;
	std::vector<std::pair<std::string, type_of_literal>> infix;
	std::vector<std::pair<std::string, type_of_literal>> postfix;

	std::vector<char> operations = { '+','-','*','/' };
	std::vector<int> priorities = { 0,0,1,1 };
	std::vector<char> numbers = { '1','2', '3', '4', '5', '6', '7', '8', '9', '0' };
	std::vector<char> special_signes = { '.', '(', ')', '-'};


	bool split();
	bool check_brackets();
	bool is_in_vector(std::vector<char>::iterator i1, std::vector<char>::iterator i2, char val);
	int operation_location(char element);

	void to_postfix();

public:
	expression() = default;
	expression(std::string str);
	expression(const expression& ex);

	std::string get_infix();
	std::string get_postfix();

	double calculate();
};