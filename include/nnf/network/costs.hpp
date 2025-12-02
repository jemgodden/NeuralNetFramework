#pragma once

#include "../matrix/matrix.hpp"


void checkContinuousCostFunctionInputs(const Matrix* actual, const Matrix* predicted);

double meanSquaredError(const Matrix* actual, const Matrix* predicted);

double continuousCrossEntropyLoss(const Matrix* actual, const Matrix* predicted);

double categoricalCrossEntropyLoss(const Matrix* actual, const Matrix* predicted);

Matrix* categoricalCrossEntropyLossDerivative(const Matrix* actual, const Matrix* predicted);
