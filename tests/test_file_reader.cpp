#include <gtest/gtest.h>

#include "../include/nnf/utils/file_reader.h"


// Testing String to Double Conversion Function:

// Test valid inputs.
TEST(StrToDoubleTest, ValidInputs) {
    EXPECT_EQ(MatrixFileReader::convertStrToDouble("0.0"), 0.0);
    EXPECT_EQ(MatrixFileReader::convertStrToDouble("4.2"), 4.2);
    EXPECT_EQ(MatrixFileReader::convertStrToDouble("-12.3"), -12.3);
    EXPECT_EQ(MatrixFileReader::convertStrToDouble("+9.9"), 9.9);
}

// Test invalid inputs.
TEST(StrToDoubleTest, InvalidInputs) {
    EXPECT_THROW(MatrixFileReader::convertStrToDouble("abc"), std::invalid_argument);
    EXPECT_THROW(MatrixFileReader::convertStrToDouble(""), std::invalid_argument);
}

// Test that leading spaces are valid.
TEST(StrToDoubleTest, ValidLeadingSpaces) {
    EXPECT_EQ(MatrixFileReader::convertStrToDouble(" \t  4.2"), 4.2);
    EXPECT_EQ(MatrixFileReader::convertStrToDouble(" -4.2"), -4.2);
}

// Test that trailing spaces are valid.
TEST(StrToDoubleTest, ValidTrailingSpaces) {
    EXPECT_EQ(MatrixFileReader::convertStrToDouble("4.2 \t "), 4.2);
    EXPECT_EQ(MatrixFileReader::convertStrToDouble("-4.2 "), -4.2);
}

// Test out of range values.
TEST(StrToDoubleTest, OutOfRange) {
    EXPECT_THROW(MatrixFileReader::convertStrToDouble("1e500"), std::out_of_range);
    EXPECT_THROW(MatrixFileReader::convertStrToDouble("-1e500"), std::out_of_range);
}

// Test boundary conditions.
TEST(StrToDoubleTest, DoubleLimits) {
    EXPECT_DOUBLE_EQ(MatrixFileReader::convertStrToDouble(std::to_string(std::numeric_limits<double>::max())), std::numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(MatrixFileReader::convertStrToDouble(std::to_string(std::numeric_limits<double>::lowest())), std::numeric_limits<double>::lowest());
}
