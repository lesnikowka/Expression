#include "expression.h"



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
	return is_in_vector(right_brackets, value);
}

bool expression::is_left_bracket(char value) {
	return is_in_vector(left_brackets, value);
}

bool expression::is_operation(char value) {
	return is_in_vector(operations, value);
}
