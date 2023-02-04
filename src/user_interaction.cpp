#include "expression.h"



void expression::request_variables() {
	double value;

	for (auto var : infix) {
		if (is_in_vector(symbols, var.first.back())) {
			if (var.first.front() == '-' && variables.find(var.first.substr(1)) == variables.end()) {
				std::cout << var.first.substr(1) << " = ";
				std::cin >> value;
				std::cout << std::endl;

				add_variable(std::make_pair(var.first.substr(1), -value));
			}

			else if (variables.find(var.first) == variables.end()) {
				std::cout << var.first << " = ";
				std::cin >> value;
				std::cout << std::endl;

				add_variable(std::make_pair(var.first, value));
			}
		}
	}
}


std::istream& operator>>(std::istream& in, expression& ex) {
	ex.clear();

	in >> ex.infix_str;

	if (!ex.split()) {
		throw "incorrect input";
	}

	ex.to_postfix();
	ex.request_variables();

	return in;
}