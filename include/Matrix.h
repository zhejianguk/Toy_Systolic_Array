#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <stdexcept>  // for std::invalid_argument

using namespace std;

class Matrix {
private:
    int** data;
    int rows;
    int cols;

public:
    Matrix(int r, int c);
    Matrix(const Matrix& other);
    ~Matrix();
    void Matrix_Zeros();
    void print() const;
    int get(int i, int j) const;
    void set(int i, int j, int val);
    int rowCount() const;
    int colCount() const;
    static Matrix multiply(const Matrix& A, const Matrix& B);
    Matrix operator*(const Matrix& B) const;
    friend Matrix Matmul(const Matrix &A, const Matrix &B);
};

#endif // MATRIX_H
