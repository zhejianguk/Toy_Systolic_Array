#include "SystolicArray.h"

SystolicArray::SystolicArray(int dim)
    : DIM(dim), matmul_count(0) {
}

int SystolicArray::getMatmulCount() const {
    return matmul_count;
}

Matrix SystolicArray::matmul(const Matrix& A, const Matrix& B) {
    if (A.rowCount() != DIM || A.colCount() != DIM ||
        B.rowCount() != DIM || B.colCount() != DIM) {
        throw invalid_argument("SystolicArray only supports DIM x DIM square matrices.");
    }

    // Multiply using your existing friend function
    Matrix C = Matmul(A, B);

    // Increment computation count
    ++matmul_count;

    return C;
}
