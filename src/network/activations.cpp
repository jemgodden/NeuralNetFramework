#include <cmath>
#include <algorithm>

#include "../../include/nnf/network/activations.h"


double square(double const value) {
    return std::pow(value, 2.0);
};

double squareDerivative(double const value) {
    return 2.0 * value;
};

double sigmoid(const double value) {
    return 1.0 / (1.0 + exp(-1.0 * value));
};

double sigmoidDerivative(const double value) {
    return value / (1.0 - value);
};

double ReLU(const double value) {
    return std::max(0.0, value);
};

double ReLUDerivative(const double value) {
    if (value <= 0.0) {
        return 0.0;
    }
    return 1.0;
};

double Tanh(const double value) {
    return (exp(value) - exp(-value)) / (exp(value) + exp(-value));
};

double TanhDerivative(const double value) {
    return 1.0 - std::pow(value, 2.0);
};
