#include "expression.h"
#include <gtest.h>

TEST(expression, can_calculate_ex_with_one_positive_operand_number) {
	expression ex("4");

	EXPECT_EQ(ex.calculate(), 4.0);
}

TEST(expression, can_calculate_ex_with_one_negative_operand_number) {
	expression ex("-4");

	EXPECT_EQ(ex.calculate(), -4.0);
}

TEST(expression, can_calculate_ex_with_one_positive_operand_variable) {
	expression ex("var", { {"var",4.0}});

	EXPECT_EQ(ex.calculate(), 4.0);
}

TEST(expression, can_calculate_ex_with_one_negative_operand_variable) {
	expression ex("var", { {"var",-4.0}});

	EXPECT_EQ(ex.calculate(), -4.0);
}

TEST(expression, can_calculate_simple_ex_with_positive_numbers) {
	expression ex("1.2+3.04+10");

	EXPECT_EQ(ex.calculate(), 14.24);
}

TEST(expression, can_calculate_simple_ex_with_negative_numbers) {
	expression ex("-1.2-3.04-10");

	EXPECT_EQ(ex.calculate(), -14.24);
}

TEST(expression, can_calculate_simple_ex_with_mixed_numbers_first) {
	expression ex("1.2-3.04+10");

	EXPECT_EQ(ex.calculate(), 8.16);
}

TEST(expression, can_calculate_simple_ex_with_mixed_numbers_second) {
	expression ex("-1.2+3.04-10");

	EXPECT_EQ(ex.calculate(), -8.16);
}

TEST(expression, can_calculate_simple_ex_with_positive_variables) {
	expression ex("a+b+c", { {"a", 1.2},{"b", 3.04},{"c", 10} });

	EXPECT_EQ(ex.calculate(), 14.24);
}

TEST(expression, can_calculate_simple_ex_with_negative_value_of_variables) {
	expression ex("a+b+c", { {"a", -1.2},{"b", -3.04},{"c", -10} });

	EXPECT_EQ(ex.calculate(), -14.24);
}

TEST(expression, can_calculate_simple_ex_with_negative_variables) {
	expression ex("-a-b-c", { {"a", 1.2},{"b", 3.04},{"c", 10} });

	EXPECT_EQ(ex.calculate(), -14.24);
}