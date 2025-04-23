#include "SystolicArray.h"
using namespace std;

SystolicArray::SystolicArray(int dim)
    : DIM(dim), matmul_count(0), last_cycles(0) {
    allocPE();
    spm.bandwith = 128;
}

SystolicArray::~SystolicArray() {
    freePE();  
}

void SystolicArray::allocPE() {
    pe = new PE*[DIM];
    for (int i = 0; i < DIM; ++i) pe[i] = new PE[DIM];
}

void SystolicArray::freePE() {
    for (int i = 0; i < DIM; ++i) delete[] pe[i];
    delete[] pe;
}

void SystolicArray::zeroRegs() {
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            pe[i][j].ia = 0;
            pe[i][j].psum = 0; 
        }
    }
}

void SystolicArray::verifyDim(const Matrix& A, const Matrix& B) const {
    if (A.rowCount()!=DIM || A.colCount()!=DIM ||
        B.rowCount()!=DIM || B.colCount()!=DIM)
        throw std::invalid_argument("SystolicArray expects DIM×DIM matrices");
}


int SystolicArray::getMatmulCount() const {
    return matmul_count;
}

int SystolicArray::getCycles() const {
    return last_cycles;
}

void SystolicArray::print_SystolicArray_W() const{
    cout << "Weights:" <<endl;
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            cout << pe[i][j].weight << "\t" << ' ';
        }
        cout << "\n" << endl;
    }
} 

void SystolicArray::print_SystolicArray_I() const{
    cout << "IAs:" <<endl;
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            cout << pe[i][j].ia << "\t" << ' ';
        cout << "\n" << endl;
        }
    }
}

void SystolicArray::print_SystolicArray_P() const{
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            cout << pe[i][j].psum << "\t" << ' ';
        }
        cout << "\n" << endl;
    }
}

// Loading Weights
void SystolicArray::streamWeights(const Matrix& B) {
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j)
            pe[i][j].weight = B.get(i, j);
        // Consumed cycles: data_size/bandwith
        last_cycles = last_cycles + ((DIM * sizeof(int)) + spm.bandwith)/spm.bandwith;
        print_SystolicArray_W();
    }
}

Matrix SystolicArray::matmul(const Matrix& A, const Matrix& B) {
    // Verify if the sizes of Matrixs and Systolic Arrary 
    verifyDim(A,B);

    // Multiply using your existing friend function
    Matrix C = Matmul(A, B);

    // Increment computation count
    ++matmul_count;

    return C;
}

Matrix SystolicArray::matmulWeightStationary(const Matrix& A, const Matrix& B) {

}