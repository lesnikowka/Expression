#pragma once

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <map>

class expression {
	enum class states_of_waiting {
		number,
		number_or_left_bracket_or_symbol,
		number_or_left_bracket_or_unary_minus_or_symbol,
		number_or_operation_or_point_or_right_bracket,
		number_or_operation_or_right_bracket,
		operation_or_right_bracket,
		symbol_or_operation_or_right_bracket,
		success
	};

	enum class type_of_literal {
		operand,
		right_bracket,
		left_bracket,
		operation,
	};

	enum class special_signes {
		point = '.',
		right_bracket = ')',
		left_bracket = '(',
		unary_minus = '-'
	};

	std::string infix_str;
	std::string postfix_str;
	std::vector<std::pair<std::string, type_of_literal>> infix;
	std::vector<std::pair<std::string, type_of_literal>> postfix;

	const std::vector<char> operations = { '+','-','*','/' };
	const std::map<char, int> priorities = { {'+',0},{'-',0},{'*',1},{'/',1} };
	const std::vector<char> numbers = { '1','2', '3', '4', '5', '6', '7', '8', '9', '0' };
	const std::vector<char> symbols = { 'q', 'Q', 'w', 'W', 'e', 'E', 'r', 'R', 't', 'T', 'y', 'Y', 'u',
		'U', 'i', 'I', 'o', 'O', 'p', 'P', 'a', 'A', 's', 'S', 'd', 'D', 'f', 'F', 'g', 'G', 'h', 'H',
		'j', 'J', 'k', 'K', 'l', 'L', 'z', 'Z', 'x', 'X', 'c', 'C', 'v', 'V', 'b', 'B', 'n', 'N', 'm', 'M', '_' };


	bool split();
	bool check_brackets();
	bool is_in_vector(std::vector<char>::const_iterator i1, std::vector<char>::const_iterator i2, char val);
	double operate(double first, double second, char operation);

	void to_postfix();

public:
	expression() = delete;
	expression(std::string str);
	expression(const expression& ex);

	std::string get_infix();
	std::string get_postfix();

	double calculate();
};