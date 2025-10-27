#include <cmath>

#include "../../include/nnf/network/activations.h"


double square(double const value) {
    return std::pow(value, 2.0);
};

double sigmoid(const double value) {
    return 1.0 / (1.0 + exp(-1.0 * value));
};
