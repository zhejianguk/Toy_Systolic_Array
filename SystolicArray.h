#ifndef SYSTOLICARRAY_H
#define SYSTOLICARRAY_H

#include <stdexcept>   // for std::invalid_argument
#include <iostream>
using namespace std;

// Forward-declare Matrix if you haven't included "Matrix.h" yet,
// but here we assume we actually need the full definition of Matrix.
#include "Matrix.h"

class SystolicArray {
private:
    int DIM;
    int matmul_count;

public:
    // Constructor: takes dimension size
    SystolicArray(int dim);

    // Getter for matmul count
    int getMatmulCount() const;

    // Perform matrix multiplication
    Matrix matmul(const Matrix& A, const Matrix& B);
};

#endif // SYSTOLICARRAY_H
