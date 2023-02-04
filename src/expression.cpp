#include "expression.h"



expression::expression(std::string str) : infix_str(str) {
	if (!split()) {
		throw "incorrect input";
	}

	to_postfix();
}

expression::expression(std::string str, std::initializer_list<std::pair<std::string, double>> list) : expression(str) {
	for (auto i : list) {
		add_variable(i);
		
		variables.insert(i);
	}
}

std::string expression::get_infix() { 
	return infix_str; 
}

std::string expression::get_postfix() { 
	return postfix_str; 
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
