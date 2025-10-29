#pragma once

#include "../matrix/matrix.h"


void checkCostFunctionInputs(const Matrix* actual, const Matrix* predicted);

static double meanSquaredError(const Matrix* actual, const Matrix* predicted);

static double crossEntropyLoss(const Matrix* actual, const Matrix* predicted);
