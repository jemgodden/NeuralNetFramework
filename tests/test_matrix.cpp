#include <gtest/gtest.h>

#include "../include/nnf/matrix/matrix.h"
#include "../include/nnf/utils/exception.h"


// Testing Matrix Functions:

// Test add throws error if input matrices are invalid.
TEST(AddTest, InvalidInputMatrices) {
    Matrix* testMatrix1 = new Matrix(3, 2);
    Matrix* testMatrix2 = new Matrix(2, 3);
    EXPECT_THROW(testMatrix1->add(testMatrix2), IllegalMatrixOperation);
}

// Test subtract throws error if input matrices are invalid.
TEST(SubtractTest, InvalidInputMatrices) {
    Matrix* testMatrix1 = new Matrix(3, 2);
    Matrix* testMatrix2 = new Matrix(2, 3);
    EXPECT_THROW(testMatrix1->subtract(testMatrix2), IllegalMatrixOperation);
}

// Test multiply throws error if input matrices are invalid.
TEST(MultiplyTest, InvalidInputMatrices) {
    Matrix* testMatrix1 = new Matrix(3, 2);
    Matrix* testMatrix2 = new Matrix(2, 3);
    EXPECT_THROW(testMatrix1->multiply(testMatrix2), IllegalMatrixOperation);
}

// Test multiply throws error if input matrices are invalid.
TEST(DotTest, InvalidInputMatrices) {
    Matrix* testMatrix1 = new Matrix(3, 3);
    Matrix* testMatrix2 = new Matrix(2, 2);
    EXPECT_THROW(testMatrix1->dot(testMatrix2), IllegalMatrixOperation);
}
