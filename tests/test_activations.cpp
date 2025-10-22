#include <gtest/gtest.h>

#include "../include/nnf/network/activations.h"


// Testing Activation Functions:

// Test valid zero input.
TEST(SigmoidTest, ZeroInput) {
    EXPECT_DOUBLE_EQ(sigmoid(0.0), 0.5);
}

// Test positive inputs approach 1.0.
TEST(SigmoidTest, PositiveInput) {
    EXPECT_NEAR(sigmoid(5.0), 1.0, 1e-2);
    EXPECT_NEAR(sigmoid(10.0), 1.0, 1e-4);
}

// Test negative inputs approach 0.0.
TEST(SigmoidTest, NegativeInput) {
    EXPECT_NEAR(sigmoid(-5.0), 0.0, 1e-2);
    EXPECT_NEAR(sigmoid(-10.0), 0.0, 1e-4);
}

// Test symmetry: sigmoid(x) = 1 - sigmoid(-x).
TEST(SigmoidTest, SymmetryProperty) {
    for (const double x : {0.5, 1.0, 2.0, 5.0}) {
        EXPECT_NEAR(sigmoid(x), 1.0 - sigmoid(-x), 1e-10);
    }
}

// Test extreme values do not cause overflow.
TEST(SigmoidTest, LargeValues) {
    EXPECT_NEAR(sigmoid(1e6), 1.0, 1e-10);
    EXPECT_NEAR(sigmoid(-1e6), 0.0, 1e-10);
}
