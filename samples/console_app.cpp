#include <iostream>
#include "expression.h"


int main() {

	expression ex;

	std::cin >> ex;

	std::cout << "Result: " << ex.calculate();
}