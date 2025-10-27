#include "calculator.h"
#include <gtest/gtest.h>
#include <exception>

TEST(CalculatorTest, AdditionWorks) {
    Calculator c;
    EXPECT_EQ(c.add(2,3), 5);
    EXPECT_DOUBLE_EQ(c.add(-1,1), 0);
}
TEST(CalculatorTest, DivisionWorks) {
    Calculator c;
    EXPECT_DOUBLE_EQ(c.divide(6, 3), 2);
}
TEST(CalculatorTest, DivisionByZeroThrows) {
    Calculator c;
    EXPECT_THROW(c.divide(10, 0), std::invalid_argument);
}
