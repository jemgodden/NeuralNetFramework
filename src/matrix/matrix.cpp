#include <iostream>
#include <stdexcept>
#include <random>
#include <tuple>
#include <mach/boolean.h>

#include "../../include/nnf/matrix/matrix.h"
#include "../../include/nnf/utils/exception.h"


Matrix::Matrix(const int rows, const int cols, const double value) {
    _rows = rows;
    _cols = cols;

    _values = static_cast<double *>(malloc(rows * cols * sizeof(double)));
    setAll(value);
};

Matrix::~Matrix() {
    free(_values);
};

int Matrix::rows() const {
    return _rows;
};

int Matrix::cols() const {
    return _cols;
};

void Matrix::setAll(const double value) const {
    for (int i=0; i<rows() * cols(); i++) {
        *(_values + i) = value;
    }
};

void Matrix::setIdentity() const {
    try {
        if (rows() != cols()) {
            throw IllegalMatrixOperation("Matrix is not square.");
        }
        for (int i=0; i<rows() * cols(); i++) {
            if (i % (rows() + 1) == 0) {
                *(_values + i) = 1;
            }
            else {
                *(_values + i) = 0;
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

void Matrix::setAscending() const {
    for (int i=0; i<rows() * cols(); i++) {
        *(_values + i) = i+1;
    }
};

void Matrix::set(const int row, const int col, const double value) const {
    *(_values + (row * cols()) + col) = value;
};

double Matrix::get(const int row, const int col) const {
    try {
        if (row >= rows()) {
            throw std::out_of_range("Row index is out of range of matrix.");
        }
        if (col >= cols()) {
            throw std::out_of_range("Column index is out of range matrix.");
        }
        return *(_values + (row * cols()) + col);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

double Matrix::_sampleUniformDistribution(const double min, const double max, const int scale) {
    const double diff = max - min;
    const int scaled_diff = static_cast<int>(diff * scale);

    std::random_device rd;
    std::mt19937 generate(rd());
    std::uniform_int_distribution<int> distribution(1, 2000000);
    return min + (1.0 * (distribution(generate) % scaled_diff) / scale);
};

void Matrix::randomise(const int n) const {
    const double min = -1.0 / sqrt(n);
    const double max = -min;

    for (int i=0; i<rows() * cols(); i++) {
        *(_values + i) = _sampleUniformDistribution(min, max);
    }
};

void Matrix::scale(const double scalar) const {
    for (int i=0; i<rows() * cols(); i++) {
        *(_values + i) *= scalar;
    }
};

void Matrix::addScalar(const double scalar) const {
    for (int i=0; i<rows() * cols(); i++) {
        *(_values + i) += scalar;
    }
};

Matrix* Matrix::rowwiseSum() const {
    try {
        if (cols() == 1) {
            throw IllegalMatrixOperation("Row-wise summations should only be done on a matrix with multiple columns");
        }

        Matrix* output = new Matrix(rows(), 1);
        for (int i=0; i<rows(); i++) {
            double curSum = 0.0;
            for (int j=0; j<cols(); j++) {
                curSum += get(i, j);
            }
            output->set(i, 0, curSum);
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

Matrix* Matrix::columnwiseSum() const {
    try {
        if (rows() == 1) {
            throw IllegalMatrixOperation("Column-wise summations should only be done on a matrix with multiple rows");
        }

        Matrix* output = new Matrix(1, cols());
        for (int j=0; j<cols(); j++) {
            double curSum = 0.0;
            for (int i=0; i<rows(); i++) {
                curSum += get(i, j);
            }
            output->set(0, j, curSum);
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

Matrix* Matrix::add(const Matrix* input) const {
    try {
        if (rows() != input->rows() || cols() != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        Matrix* output = new Matrix(rows(), cols());
        for (int i=0; i<rows(); i++) {
            for (int j=0; j<cols(); j++) {
                const double value = get(i, j) + input->get(i, j);
                output->set(i, j, value);
            }
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

Matrix* Matrix::rowwiseAdd(const Matrix* input) const {
    try {
        if (cols() != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions are invalid.");
        }
        Matrix* output = new Matrix(rows(), cols());
        for (int i=0; i<rows(); i++) {
            for (int j=0; j<cols(); j++) {
                const double value = get(i, j) + input->get(0, j);
                output->set(i, j, value);
            }
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

Matrix* Matrix::columnwiseAdd(const Matrix* input) const {
    try {
        if (rows() != input->rows()) {
            throw IllegalMatrixOperation("Matrix dimensions are invalid.");
        }
        Matrix* output = new Matrix(rows(), cols());
        for (int i=0; i<rows(); i++) {
            for (int j=0; j<cols(); j++) {
                const double value = get(i, j) + input->get(i, 0);
                output->set(i, j, value);
            }
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

Matrix* Matrix::subtract(const Matrix* input) const {
    try {
        if (rows() != input->rows() || cols() != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        Matrix* output = new Matrix(rows(), cols());
        for (int i=0; i<rows(); i++) {
            for (int j=0; j<cols(); j++) {
                const double value = get(i, j) - input->get(i, j);
                output->set(i, j, value);
            }
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

Matrix* Matrix::multiply(const Matrix* input) const {
    try {
        if (rows() != input->rows() || cols() != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        Matrix* output = new Matrix(rows(), cols());
        for (int i=0; i<rows(); i++) {
            for (int j=0; j<cols(); j++) {
                const double value = get(i, j) * input->get(i, j);
                output->set(i, j, value);
            }
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

Matrix* Matrix::rowwiseMultiply(const Matrix* input) const {
    try {
        if (input->rows() != 1 || cols() != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        Matrix* output = new Matrix(rows(), cols());
        for (int i=0; i<rows(); i++) {
            for (int j=0; j<cols(); j++) {
                const double value = get(i, j) * input->get(0, j);
                output->set(i, j, value);
            }
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

Matrix* Matrix::columnwiseMultiply(const Matrix* input) const {
    try {
        if (rows() != input->rows() || input->cols() != 1) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        Matrix* output = new Matrix(rows(), cols());
        for (int i=0; i<rows(); i++) {
            for (int j=0; j<cols(); j++) {
                const double value = get(i, j) * input->get(i, 0);
                output->set(i, j, value);
            }
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

Matrix* Matrix::dot(const Matrix* input) const {
    try {
        if (cols() != input->rows()) {
            throw IllegalMatrixOperation("Matrix dimensions are not compatible.");
        }
        Matrix* output = new Matrix(rows(), input->cols());
        for (int i=0; i<rows(); i++) {
            for (int j=0; j<input->cols(); j++) {
                double sum = 0;
                for (int k=0; k<cols(); k++) {
                    sum += get(i, k) * input->get(k, j);
                }
                output->set(i, j, sum);
            }
        }
        return output;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

void Matrix::apply(double (*func)(double)) const {
    for (int i=0; i<rows() * cols(); i++) {
        *(_values + i) = (*func)(*(_values + i));
    }
};

void Matrix::transpose() const {
    for (int i=0; i<rows(); i++) {
        for (int j=0; j<cols(); j++) {
            if (j > i) {
                const double saved = get(i, j);
                set(i, j, get(j, i));
                set(j, i, saved);
            }
        }
    }
};

Matrix* Matrix::rowwiseArgmax() const {
    if (rows() > 1) {
        throw IllegalMatrixOperation("Row-wise Argmax should only be done on a matrix with multiple rows.");
    }

    Matrix* output = new Matrix(rows(), 1);

    for (int i=0; i<rows(); i++) {
        double rowMax = get(i, 0);
        int rowMaxIndex = 0;
        for (int j=1; j<cols(); j++) {
            const double rowElement = get(i, j);
            if (rowElement > rowMax) {
                rowMax = rowElement;
                rowMaxIndex = j;
            }
        }
        output->set(i, 0, (rowMaxIndex * 1.0)); // Multiplying by 1.0 to cast to double.
    }

    return output;
};

Matrix* Matrix::columnwiseArgmax() const {
    if (cols() > 1) {
        throw IllegalMatrixOperation("Column-wise Argmax should only be done on a matrix with multiple rows.");
    }

    Matrix* output = new Matrix(1, cols());

    for (int j=0; j<cols(); j++) {
        double colMax = get(0, j);
        int colMaxIndex = 0;
        for (int i=1; i<rows(); i++) {
            const double colElement = get(i, j);
            if (colElement > colMax) {
                colMax = colElement;
                colMaxIndex = i;
            }
        }
        output->set(0, j, (colMaxIndex * 1.0)); // Multiplying by 1.0 to cast to double.
    }

    return output;
};

Matrix* Matrix::row(const int row) const {
    if (row < 0 || row >= rows()) {
        throw IllegalMatrixOperation("Given row index is invalid.");
    }

    Matrix* slice = new Matrix(1, cols());

    for (int i=0; i<cols(); i++) {
        slice->set(0, i, get(row, i));
    }

    return slice;
};

Matrix* Matrix::col(const int col) const {
    if (col < 0 || col >= cols()) {
        throw IllegalMatrixOperation("Given col index is invalid.");
    }

    Matrix* slice = new Matrix(rows(), 1);

    for (int i=0; i<rows(); i++) {
        slice->set(i, 0, get(i, col));
    }

    return slice;
};

std::tuple<Matrix*, Matrix*> Matrix::rowSlice(const int row) const {
    if (row < 0 || row >= rows()) {
        throw IllegalMatrixOperation("Given row index is invalid.");
    }

    Matrix* slice = new Matrix(1, cols());
    Matrix* nonSlice = new Matrix(rows()-1, cols());

    int passedSliceRow = FALSE;
    for (int i=0; i<rows(); i++) {
        if (i == row) {
            passedSliceRow = TRUE;
        }
        for (int j=0; j<cols(); j++) {
            if (i == row) {
                slice->set(0, j, get(i, j));
            }
            else {
                nonSlice->set(i-passedSliceRow, j, get(i, j));
            }
        }
    }
    return {slice, nonSlice};
};

std::tuple<Matrix*, Matrix*> Matrix::colSlice(const int col) const {
    if (col < 0 || col >= cols()) {
        throw IllegalMatrixOperation("Given col index is invalid.");
    }

    Matrix* slice = new Matrix(rows(), 1);
    Matrix* nonSlice = new Matrix(rows(), cols()-1);

    for (int i=0; i<rows(); i++) {
        int passedSliceCol = FALSE;
        for (int j=0; j<cols(); j++) {
            if (j == col) {
                passedSliceCol = TRUE;
                slice->set(i, 0, get(i, j));
            }
            else {
                nonSlice->set(i, j-passedSliceCol, get(i, j));
            }
        }
    }
    return {slice, nonSlice};
};

Matrix* Matrix::deepCopy() const {
    Matrix* output = new Matrix(rows(), cols());
    for (int i = 0; i<rows(); i++) {
        for (int j = 0; j<cols(); j++) {
            output->set(i, j, get(i, j));
        }
    }
    return output;
};

void Matrix::print() const {
    for (int i = 0; i<rows(); i++) {
        for (int j = 0; j<cols(); j++) {
            std::cout << get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
};
