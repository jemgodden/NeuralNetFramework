#pragma once

#define DEFAULT_VALUE 0


class Matrix {
    private:
        int _rows;
        int _cols;
        double* _values;

        static double _uniform_distribution(double min, double max, int scale=10000);

    public:
        Matrix(int rows, int cols, double value=DEFAULT_VALUE);
        
        ~Matrix();
        
        int rows() const;
        
        int cols() const;

        void setAll(double value=DEFAULT_VALUE) const;
        
        void setIdentity() const;
        
        void setAscending() const;

        void set(int row, int col, double value) const;

        double get(int row, int col) const;

        void randomise(int n) const;

        void scale(double scalar) const;
        
        void addScalar(double scalar) const;
        
        void subtractScalar(double scalar) const;
        
        Matrix* add(const Matrix* input) const;
        
        Matrix* subtract(const Matrix* input) const;
        
        Matrix* multiply(const Matrix* input) const;
        
        Matrix* dot(const Matrix* input) const;
        
        void apply(double (*func)(double)) const;
        
        void transpose() const;

        void print() const;
};