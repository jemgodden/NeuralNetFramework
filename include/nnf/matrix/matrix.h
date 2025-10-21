#pragma once

#define DEFAULT_VALUE 0


class Matrix {
    private:
        int _rows;
        int _cols;
        double* _values;

    public:
        Matrix(int rows, int cols, double value=DEFAULT_VALUE);
        
        ~Matrix();
        
        int rows() const;
        
        int cols() const;

        void clear(double value=DEFAULT_VALUE) const;
        
        void setIdentity() const;
        
        void setAscending() const;

        void set(int row, int col, double value) const;

        double get(int row, int col);
        
        void scale(double scalar);
        
        void addScalar(double scalar);
        
        void subtractScalar(double scalar);
        
        Matrix* add(Matrix* input);
        
        Matrix* subtract(Matrix* input);
        
        Matrix* multiply(Matrix* input);
        
        Matrix* dot(Matrix* m2);
        
        void apply(double (*func)(double));
        
        void transpose();

        void print();
};