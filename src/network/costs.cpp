#include <iostream>
#include <cmath>

#include "../../include/nnf/matrix/matrix.h"
#include "../../include/nnf/network/costs.h"
#include "../../include/nnf/utils/exception.h"


void checkCostFunctionInputs(const Matrix* actual, const Matrix* predicted) {
    if (actual->cols() > 1 || predicted->cols() > 1) {
        throw IllegalMatrixOperation("Cost functions can only be calculated on two column vectors.");
    }
    if (actual->rows() != predicted->rows()) {
        throw IllegalMatrixOperation("Cost functions can only be calculated on two column vectors of the same length.");
    }
};

double meanSquaredError(const Matrix* actual, const Matrix* predicted) {
    try {
        checkCostFunctionInputs(actual, predicted);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }

    double curSum = 0;
    for (int i=0; i<actual->rows(); i++) {
        curSum += std::pow(actual->get(i, 1) - predicted->get(i, 1), 2.0);
    }
    return 0.5 * curSum;
};

double crossEntropyLoss(const Matrix* actual, const Matrix* predicted) {
    try {
        checkCostFunctionInputs(actual, predicted);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }

    double curSum = 0;
    for (int i=0; i<actual->rows(); i++) {
        curSum += actual->get(i, 1) * std::log(predicted->get(i, 1));
    }
    return -curSum;
};
