#include "expression.h"
#include <gtest.h>

TEST(expression, throw_if_div_by_zero_number) {
	expression ex("1/0");
	ASSERT_ANY_THROW(ex.calculate());
}

TEST(expression, throw_if_div_by_zero_value) {
	expression ex("1/(2-2)");
	ASSERT_ANY_THROW(ex.calculate());
}

TEST(expression, throw_if_div_by_zero_variable) {
	expression ex("1/a", { {"a", 0.0}});
	ASSERT_ANY_THROW(ex.calculate());
}

TEST(expression, throw_if_div_by_zero_variable_value) {
	expression ex("1/(a-b)", { {"a", 2}, {"b", 2} });
	ASSERT_ANY_THROW(ex.calculate());
}

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

TEST(expression, can_calculate_ex_with_unary_minus_1) {
	expression ex("-(-1)");

	EXPECT_EQ(ex.calculate(), 1.0);
}

TEST(expression, can_calculate_ex_with_unary_minus_2) {
	expression ex("-(-(-1))");

	EXPECT_EQ(ex.calculate(), -1.0);
}

TEST(expression, can_calculate_ex_with_operation_and_unary_minus_1) {
	expression ex("2-(-1)");

	EXPECT_EQ(ex.calculate(), 3.0);
}

TEST(expression, can_calculate_ex_with_operation_and_unary_minus_2) {
	expression ex("2-(-(-1))");

	EXPECT_EQ(ex.calculate(), 1.0);
}

TEST(expression, can_calculate_ex_with_unary_minus_1_variable) {
	expression ex("-(-a)", { {"a",1.0}});

	EXPECT_EQ(ex.calculate(), 1.0);
}

TEST(expression, can_calculate_ex_with_unary_minus_2_variable) {
	expression ex("-(-(-a))", { {"a",1.0}});

	EXPECT_EQ(ex.calculate(), -1.0);
}

TEST(expression, can_calculate_ex_with_operation_and_unary_minus_1_variable) {
	expression ex("2-(-a)", { {"a",1.0} });

	EXPECT_EQ(ex.calculate(), 3.0);
}

TEST(expression, can_calculate_ex_with_operation_and_unary_minus_2_variable) {
	expression ex("2-(-(-a))", { {"a",1.0} });

	EXPECT_EQ(ex.calculate(), 1.0);
}

TEST(expression, can_calculate_simple_ex_with_positive_numbers) {
	expression ex("1.2+3.04+10");

	EXPECT_EQ(ex.calculate(), 14.24);
}

TEST(expression, can_calculate_simple_ex_with_negative_numbers) {
	expression ex("-1.2-3.04-10");

	EXPECT_EQ(ex.calculate(), -14.24);
}

TEST(expression, can_calculate_simple_ex_with_mixed_numbers_1) {
	expression ex("1.2-3.04+10");

	EXPECT_EQ(ex.calculate(), 8.16);
}

TEST(expression, can_calculate_simple_ex_with_mixed_numbers_2) {
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

TEST(expression, can_calculate_simple_ex_with_mixed_signes_of_variables) {
	expression ex("-a+b-c", { {"a", 1.2},{"b", 3.04},{"c", 10} });

	EXPECT_EQ(ex.calculate(), -8.16);
}

TEST(expression, can_calculate_simple_ex_with_mixed_signes_of_value_of_variables_and_variables) {
	expression ex("-a+b-c", { {"a", -1.2},{"b", 3.04},{"c", -10} });

	EXPECT_EQ(ex.calculate(), 14.24);
}

TEST(expression, can_devide_positive_number_by_positive_number) {
	expression ex("2/8");

	EXPECT_EQ(ex.calculate(), 0.25);
}

TEST(expression, can_devide_positive_number_by_negative_number) {
	expression ex("2/(-8)");

	EXPECT_EQ(ex.calculate(), -0.25);
}

TEST(expression, can_devide_negative_number_by_positive_number) {
	expression ex("-2/8");

	EXPECT_EQ(ex.calculate(), -0.25);
}

TEST(expression, can_devide_negative_number_by_negative_number) {
	expression ex("-2/(-8)");

	EXPECT_EQ(ex.calculate(), 0.25);
}

TEST(expression, can_devide_positive_variable_by_positive_variable) {
	expression ex("a/b",{ {"a",2},{"b",8}});

	EXPECT_EQ(ex.calculate(), 0.25);
}

TEST(expression, can_devide_positive_variable_by_negative_variable) {
	expression ex("a/(-b)", { {"a",2},{"b",8} });

	EXPECT_EQ(ex.calculate(), -0.25);
}

TEST(expression, can_devide_negative_variable_by_positive_variable) {
	expression ex("-a/b", { {"a",2},{"b",8} });

	EXPECT_EQ(ex.calculate(), -0.25);
}

TEST(expression, can_devide_negative_variable_by_negative_variable) {
	expression ex("-a/(-b)", { {"a",2},{"b",8} });

	EXPECT_EQ(ex.calculate(), 0.25);
}

//////////////////
TEST(expression, can_multiply_positive_number_by_positive_number) {
	expression ex("2*8");

	EXPECT_EQ(ex.calculate(), 16.0);
}

TEST(expression, can_multiply_positive_number_by_negative_number) {
	expression ex("2*(-8)");

	EXPECT_EQ(ex.calculate(), -16.0);
}

TEST(expression, can_multiply_negative_number_by_positive_number) {
	expression ex("-2*8");

	EXPECT_EQ(ex.calculate(), -16.0);
}

TEST(expression, can_multiply_negative_number_by_negative_number) {
	expression ex("-2*(-8)");

	EXPECT_EQ(ex.calculate(), 16.0);
}

TEST(expression, can_multiply_positive_variable_by_positive_variable) {
	expression ex("a*b", { {"a",2},{"b",8} });

	EXPECT_EQ(ex.calculate(), 16.0);
}

TEST(expression, can_multiply_positive_variable_by_negative_variable) {
	expression ex("a*(-b)", { {"a",2},{"b",8} });

	EXPECT_EQ(ex.calculate(), -16.0);
}

TEST(expression, can_multiply_negative_variable_by_positive_variable) {
	expression ex("-a*b", { {"a",2},{"b",8} });

	EXPECT_EQ(ex.calculate(), -16.0);
}

TEST(expression, can_multiply_negative_variable_by_negative_variable) {
	expression ex("-a*(-b)", { {"a",2},{"b",8} });

	EXPECT_EQ(ex.calculate(), 16.0);
}

TEST(expression, throw_ex_with_wrong_brackets_1) {
	expression ex("(()");

	ASSERT_ANY_THROW(ex.calculate());
}

TEST(expression, throw_ex_with_wrong_brackets_2) {
	expression ex(")(");

	ASSERT_ANY_THROW(ex.calculate());
}

TEST(expression, throw_ex_with_wrong_brackets_3) {
	expression ex("())");

	ASSERT_ANY_THROW(ex.calculate());
}

TEST(expression, throw_ex_with_wrong_brackets_4) {
	expression ex("()(");

	ASSERT_ANY_THROW(ex.calculate());
}

TEST(expression, throw_ex_with_wrong_brackets_5) {
	expression ex(")()");

	ASSERT_ANY_THROW(ex.calculate());
}

TEST(expression, can_add_and_multiply_correctly) {
	expression ex("1+2*3");

	EXPECT_EQ(ex.calculate(), 7);
}

TEST(expression, can_add_and_devide_correctly) {
	expression ex("1+1/2");

	EXPECT_EQ(ex.calculate(), 1.5);
}