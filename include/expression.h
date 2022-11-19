#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <iostream>

class expression {
	std::string infix_str;
	std::vector<std::string> infix;
	std::vector<std::string> postfix;

	std::vector<char> operations = { '+','-','*','/' };
	std::vector<int> priorities = { 0,0,1,1 };
	std::vector<char> numbers = { '1','2', '3', '4', '5', '6', '7', '8', '9', '0' };
	std::vector<char> special_signes = { '.', '(', ')', '-'};

	enum states_of_waiting{
		number,
		number_or_left_bracket,
		number_or_operation_or_point_or_right_bracket,
		operation_or_right_bracket,		
	};

	bool split();
	bool check_brackets();
	bool is_in_vector(std::vector<char>::iterator i1, std::vector<char>::iterator i2, char val);

public:
	expression() = default;
	expression(std::string str);
	expression(const expression& ex);

	std::vector<std::string> get_infix();
	std::vector<std::string> get_postfix();

	double calculate();
};