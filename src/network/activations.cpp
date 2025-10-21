#include <cmath>

#include "../../include/nnffs/network/activations.h"


double sigmoid(double input) {
    return 1.0 / (1.0 + exp(-1.0 * input));
}
