#include <iostream>
#include <stdexcept>

#include "../../include/nnffs/matrix/matrix.h"
#include "../../include/nnffs/utils/exception.h"


Matrix::Matrix(int const rows, int const cols, double const value) {
    _rows = rows;
    _cols = cols;

    _values = static_cast<double *>(malloc(_rows * _cols * sizeof(double)));
    clear(value);
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

void Matrix::clear(double const value) const {
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) = value;
    }
};

void Matrix::setIdentity() const {
    try {
        if (_rows != _cols) {
            throw IllegalMatrixOperation("Matrix is not square.");
        }
        else {
            for (int i=0; i<_rows * _cols; i++) {
                if (i % (_rows + 1) == 0) {
                    *(_values + i) = 1;
                }
                else {
                    *(_values + i) = 0;
                }
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl; 
    }
};

void Matrix::setAscending() const {
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) = i+1;
    }
};

void Matrix::set(int row, int col, double value) const {
    *(_values + (row * _cols) + col) = value;
};

double Matrix::get(int row, int col) {
    try {
        if (row >= _rows) {
            throw std::out_of_range("Row index is out of range of matrix.");
        }
        else if (col >= _cols) {
            throw std::out_of_range("Column index is out of range matirx.");
        }
        else {
            return *(_values + (row * _cols) + col);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
};

void Matrix::scale(double scalar) {
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) *= scalar;
    }
};

void Matrix::addScalar(double scalar) {
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) += scalar;
    }
};

void Matrix::subtractScalar(double scalar) {
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) -= scalar;
    }
};

Matrix* Matrix::add(Matrix* input) {
    try {
        if (_rows != input->rows() || _cols != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        else {
            Matrix* output = new Matrix(_rows, _cols);
            for (int i=0; i<_rows; i++) {
                for (int j=0; j<_cols; j++) {
                    double value = get(i, j) + input->get(i, j);
                    output->set(i, j, value);
                }
            }
            return output;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl; 
    }
};

Matrix* Matrix::subtract(Matrix* input) {
    try {
        if (_rows != input->rows() || _cols != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        else {
            Matrix* output = new Matrix(_rows, _cols);
            for (int i=0; i<_rows; i++) {
                for (int j=0; j<_cols; j++) {
                    double value = get(i, j) - input->get(i, j);
                    output->set(i, j, value);
                }
            }
            return output;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl; 
    }
};

Matrix* Matrix::multiply(Matrix* input) {
    try {
        if (_rows != input->rows() || _cols != input->cols()) {
            throw IllegalMatrixOperation("Matrix dimensions do not match.");
        }
        else {
            Matrix* output = new Matrix(_rows, _cols);
            for (int i=0; i<_rows; i++) {
                for (int j=0; j<_cols; j++) {
                    double value = get(i, j) * input->get(i, j);
                    output->set(i, j, value);
                }
            }
            return output;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl; 
    }
};

Matrix* Matrix::dot(Matrix* input) {
    try {
        if (_cols != input->rows()) {
            throw IllegalMatrixOperation("Matrix dimensions are not compatible.");
        }
        else {
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
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl; 
    }
};

void Matrix::apply(double (*func)(double)) {
    for (int i=0; i<_rows * _cols; i++) {
        *(_values + i) = (*func)(*(_values + i));
    }
};

void Matrix::transpose() {
    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            if (j > i) {
                double saved = get(i, j);
                set(i, j, get(j, i));
                set(j, i, saved);
            }
        }
    }
};

void Matrix::print() {
    for (int i = 0; i<_rows; i++) {
        for (int j = 0; j<_cols; j++) {
            std::cout << Matrix::get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
};