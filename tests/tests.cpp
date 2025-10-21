#include "gtest/gtest.h"

#include "include/nnffs/matrix/activations.h"

TEST(Activations, ExpectedValue) {
    EXPECT_FLOAT_EQ(sigmoid(0), 0.5);
}
