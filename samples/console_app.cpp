#include <iostream>
#include "expression.h"


int main() {
	while (true)
	{
		try
		{
			expression ex;
			std::cin >> ex;
			std::cout << "Result: " << ex.calculate() << std::endl;
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}
}