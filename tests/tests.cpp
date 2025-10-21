#include <gtest/gtest.h>

#include "../include/nnf/network/activations.h"


TEST(Activations, ExpectedValue) {
    EXPECT_FLOAT_EQ(sigmoid(0), 0.5);
}
