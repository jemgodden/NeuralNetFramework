#include <gtest/gtest.h>

#include "../include/nnf/utils/dataset.h"


// Testing String to Int Conversion Function:

// Test valid inputs.
TEST(StrToIntTest, ValidInputs) {
    EXPECT_EQ(DataSet::convertStrToInt("0"), 0);
    EXPECT_EQ(DataSet::convertStrToInt("42"), 42);
    EXPECT_EQ(DataSet::convertStrToInt("-123"), -123);
    EXPECT_EQ(DataSet::convertStrToInt("+99"), 99);
}

// Test invalid inputs.
TEST(StrToIntTest, InvalidInputs) {
    EXPECT_THROW(DataSet::convertStrToInt("abc"), std::invalid_argument);
    EXPECT_THROW(DataSet::convertStrToInt(""), std::invalid_argument);
}

// Test that leading spaces are valid.
TEST(StrToIntTest, ValidLeadingSpaces) {
    EXPECT_EQ(DataSet::convertStrToInt(" \t  42"), 42);
    EXPECT_EQ(DataSet::convertStrToInt(" -42"), -42);
}

// Test that trailing spaces are valid.
TEST(StrToIntTest, ValidTrailingSpaces) {
    EXPECT_EQ(DataSet::convertStrToInt("42 \t "), 42);
    EXPECT_EQ(DataSet::convertStrToInt("-42 "), -42);
}

// Test out of range values.
TEST(StrToIntTest, OutOfRange) {
    EXPECT_THROW(DataSet::convertStrToInt("999999999999999999999999"), std::out_of_range);
    EXPECT_THROW(DataSet::convertStrToInt("-999999999999999999999999"), std::out_of_range);
}

// Test boundary conditions.
TEST(StrToIntTest, IntLimits) {
    EXPECT_EQ(DataSet::convertStrToInt(std::to_string(std::numeric_limits<int>::max())), std::numeric_limits<int>::max());
    EXPECT_EQ(DataSet::convertStrToInt(std::to_string(std::numeric_limits<int>::min())), std::numeric_limits<int>::min());
}

// Testing String to Double Conversion Function:

// Test valid inputs.
TEST(StrToDoubleTest, ValidInputs) {
    EXPECT_EQ(DataSet::convertStrToDouble("0.0"), 0.0);
    EXPECT_EQ(DataSet::convertStrToDouble("4.2"), 4.2);
    EXPECT_EQ(DataSet::convertStrToDouble("-12.3"), -12.3);
    EXPECT_EQ(DataSet::convertStrToDouble("+9.9"), 9.9);
}

// Test invalid inputs.
TEST(StrToDoubleTest, InvalidInputs) {
    EXPECT_THROW(DataSet::convertStrToDouble("abc"), std::invalid_argument);
    EXPECT_THROW(DataSet::convertStrToDouble(""), std::invalid_argument);
}

// Test that leading spaces are valid.
TEST(StrToDoubleTest, ValidLeadingSpaces) {
    EXPECT_EQ(DataSet::convertStrToDouble(" \t  4.2"), 4.2);
    EXPECT_EQ(DataSet::convertStrToDouble(" -4.2"), -4.2);
}

// Test that trailing spaces are valid.
TEST(StrToDoubleTest, ValidTrailingSpaces) {
    EXPECT_EQ(DataSet::convertStrToDouble("4.2 \t "), 4.2);
    EXPECT_EQ(DataSet::convertStrToDouble("-4.2 "), -4.2);
}

// Test out of range values.
TEST(StrToDoubleTest, OutOfRange) {
    EXPECT_THROW(DataSet::convertStrToDouble("1e500"), std::out_of_range);
    EXPECT_THROW(DataSet::convertStrToDouble("-1e500"), std::out_of_range);
}

// Test boundary conditions.
TEST(StrToDoubleTest, DoubleLimits) {
    EXPECT_DOUBLE_EQ(DataSet::convertStrToDouble(std::to_string(std::numeric_limits<double>::max())), std::numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(DataSet::convertStrToDouble(std::to_string(std::numeric_limits<double>::lowest())), std::numeric_limits<double>::lowest());
}
