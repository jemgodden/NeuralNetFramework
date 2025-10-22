#include <cmath>

#include "../../include/nnf/network/activations.h"


double sigmoid(const double input) {
    return 1.0 / (1.0 + exp(-1.0 * input));
}
