#pragma once

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <initializer_list>
#include <iostream>
#include <algorithm>

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
		unary_minus = '-'
	};

	std::string infix_str;
	std::string postfix_str;
	std::vector<std::pair<std::string, type_of_literal>> infix;
	std::vector<std::pair<std::string, type_of_literal>> postfix;

	const std::vector<char> operations = { '+','-','*','/' };
	const std::map<char, int> priorities = { {'+',0},{'-',0},{'*',1},{'/',1} };
	const std::vector<char> left_brackets = { '(','[','{' };
	const std::vector<char> right_brackets = { ')',']','}' };
	const std::vector<char> numbers = { '1','2', '3', '4', '5', '6', '7', '8', '9', '0' };
	const std::vector<char> symbols = { 'q', 'Q', 'w', 'W', 'e', 'E', 'r', 'R', 't', 'T', 'y', 'Y', 'u',
		'U', 'i', 'I', 'o', 'O', 'p', 'P', 'a', 'A', 's', 'S', 'd', 'D', 'f', 'F', 'g', 'G', 'h', 'H',
		'j', 'J', 'k', 'K', 'l', 'L', 'z', 'Z', 'x', 'X', 'c', 'C', 'v', 'V', 'b', 'B', 'n', 'N', 'm', 'M', '_' };

	
	std::map<std::string, double> constants = { {"pi",3.14159265358979323846},
												{"e", 2.71828182845904523536}
												};
	std::map<std::string, double> variables = constants;
	
	bool split();
	bool check_brackets();
	bool is_in_vector(const std::vector<char>& v, char value);
	double operate(double first, double second, char operation);
	void request_variables();

	void to_postfix();

	void clear();

public:
	expression() = default;
	expression(std::string str);
	expression(const expression& ex)=default;
	expression(std::string str, std::initializer_list<std::pair<std::string, double>> list);

	void add_variable(std::pair<std::string, double> var);

	void change_expression(std::string ex);

	friend std::istream& operator>>(std::istream& in, expression& ex) {
		ex.clear();
		in >> ex.infix_str;
		if (!ex.split())
			throw "incorrect input";
		ex.to_postfix();
		ex.request_variables();
		return in;
	}

	std::string get_infix() { return infix_str; }
	std::string get_postfix() { return postfix_str; }

	double calculate();
};