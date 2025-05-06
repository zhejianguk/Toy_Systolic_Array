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
    struct PE {int weight, ia, psum;};
    struct SPM {int bandwith;};
    int DIM;
    int matmul_count;
    PE** pe = NULL;
    SPM  spm;
    int  last_cycles;
    

    // Play with PEs for the Systolic Array
    void  allocPE();
    void  freePE();
    void  zeroRegs();                       // clear ia / psum

public:
    SystolicArray(int dim);
    ~SystolicArray();

    // Print Info
    int getMatmulCount() const;
    int getCycles() const;
    void print_SystolicArray_W() const;
    void print_SystolicArray_I() const;
    void print_SystolicArray_P() const;
    
    void  preLoadWeights(const Matrix& B);   // simulate loading B (returns cycles)
    Matrix runCompute(const Matrix& A);      // feed A & MAC (adds cycles)
    Matrix runOutputStationary(const Matrix &A, const Matrix& B);   // simulate loading B (returns cycles)

    void  verifyDim(const Matrix&, const Matrix&) const;

    // Perform matrix multiplication
    Matrix matmul(const Matrix& A, const Matrix& B);
    Matrix matmulWeightStationary(const Matrix& A, const Matrix& B);
};

#endif // SYSTOLICARRAY_H
