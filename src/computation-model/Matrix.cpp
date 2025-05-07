#include "Matrix.h"

Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    data = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new int[cols];
    }

    // Initialize with increasing numbers
    int val = 1;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = val++;
        }
    }
}

Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
    data = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < rows; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

void Matrix::Matrix_Zeros() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = 0;
        }
    }
}

void Matrix::print() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << data[i][j] << "\t";
        }
        cout << endl;
    }
}

int Matrix::get(int i, int j) const {
    return data[i][j];
}

void Matrix::set(int i, int j, int val) {
    data[i][j] = val;
}

int Matrix::rowCount() const {
    return rows;
}

int Matrix::colCount() const {
    return cols;
}

Matrix Matrix::multiply(const Matrix& A, const Matrix& B) {
    return A * B;
}

Matrix Matrix::operator*(const Matrix& B) const {
    if (cols != B.rows) {
        throw invalid_argument("Matrix multiplication error: A's columns must equal B's rows.");
    }

    Matrix result(rows, B.cols);
    result.Matrix_Zeros();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < B.cols; ++j) {
            for (int k = 0; k < cols; ++k) {
                result.set(i, j, result.get(i, j) + data[i][k] * B.get(k, j));
            }
        }
    }
    return result;
}

// Friend function
Matrix Matmul(const Matrix &A, const Matrix &B) {
    if (A.cols != B.rows) {
        throw invalid_argument("Matrix multiplication error: A's columns must equal B's rows.");
    }

    Matrix C(A.rows, B.cols);
    C.Matrix_Zeros();

    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < B.cols; ++j) {
            for (int k = 0; k < A.cols; ++k) {
                C.set(i, j, C.get(i, j) + A.get(i, k) * B.get(k, j));
            }
        }
    }

    return C;
}
