#include <gtest.h>
#include "expression.h"
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();

	//expression ex("-a-b-c", { {"a",1},{"b",2},{"c",3} });
	//
	//std::cout << ex.calculate();
}
