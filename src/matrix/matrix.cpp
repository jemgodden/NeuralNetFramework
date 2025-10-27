#include <iostream>
#include <stdexcept>
#include <random>

#include "../../include/nnf/matrix/matrix.h"
#include "../../include/nnf/utils/exception.h"


Matrix::Matrix(const int rows, const int cols, const double value) {
    _rows = rows;
    _cols = cols;

    _values = static_cast<double *>(malloc(_rows * _cols * sizeof(double)));
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
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) = value;
    }
};

void Matrix::setIdentity() const {
    try {
        if (_rows != _cols) {
            throw IllegalMatrixOperation("Matrix is not square.");
        }
        for (int i=0; i<_rows * _cols; i++) {
            if (i % (_rows + 1) == 0) {
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
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) = i+1;
    }
};

void Matrix::set(const int row, const int col, const double value) const {
    *(_values + (row * _cols) + col) = value;
};

double Matrix::get(const int row, const int col) const {
    try {
        if (row >= _rows) {
            throw std::out_of_range("Row index is out of range of matrix.");
        }
        if (col >= _cols) {
            throw std::out_of_range("Column index is out of range matrix.");
        }
        return *(_values + (row * _cols) + col);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
};

double Matrix::_uniform_distribution(const double min, const double max, const int scale) {
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

    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) = _uniform_distribution(min, max);
    }
};

void Matrix::scale(const double scalar) const {
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) *= scalar;
    }
};

void Matrix::addScalar(const double scalar) const {
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) += scalar;
    }
};

void Matrix::subtractScalar(const double scalar) const {
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) -= scalar;
    }
};

Matrix* Matrix::add(const Matrix* input) const {
    try {
        if (_rows != input->rows() || _cols != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        Matrix* output = new Matrix(_rows, _cols);
        for (int i=0; i<_rows; i++) {
            for (int j=0; j<_cols; j++) {
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

Matrix* Matrix::subtract(const Matrix* input) const {
    try {
        if (_rows != input->rows() || _cols != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        Matrix* output = new Matrix(_rows, _cols);
        for (int i=0; i<_rows; i++) {
            for (int j=0; j<_cols; j++) {
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
        if (_rows != input->rows() || _cols != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        Matrix* output = new Matrix(_rows, _cols);
        for (int i=0; i<_rows; i++) {
            for (int j=0; j<_cols; j++) {
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

Matrix* Matrix::dot(const Matrix* input) const {
    try {
        if (_cols != input->rows()) {
            throw IllegalMatrixOperation("Matrix dimensions are not compatible.");
        }
        Matrix* output = new Matrix(_rows, input->cols());
        for (int i=0; i<_rows; i++) {
            for (int j=0; j<input->cols(); j++) {
                double sum = 0;
                for (int k=0; k<_cols; k++) {
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
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) = (*func)(*(_values + i));
    }
};

void Matrix::transpose() const {
    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            if (j > i) {
                const double saved = get(i, j);
                set(i, j, get(j, i));
                set(j, i, saved);
            }
        }
    }
};

void Matrix::print() const {
    for (int i = 0; i<_rows; i++) {
        for (int j = 0; j<_cols; j++) {
            std::cout << get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
};
