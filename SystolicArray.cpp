#include "Top.h"
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
        }
        cout << "\n" << endl;
    }
}

void SystolicArray::print_SystolicArray_P() const{
    cout << "PSUMs:" <<endl;
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            cout << pe[i][j].psum << "\t" << ' ';
        }
        cout << "\n" << endl;
    }
}

// Loading Weights
void SystolicArray::PreloadWeights(const Matrix& B) {
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j)
            pe[i][j].weight = B.get(i, j);
        // Consumed cycles: data_size/bandwith
        last_cycles = last_cycles + ((DIM * sizeof(int)) + spm.bandwith)/spm.bandwith;
        // print_SystolicArray_W();
    }
}

Matrix SystolicArray::runCompute(const Matrix& A) {
    const int MOV = 2*DIM - 1;
    Matrix C(DIM, DIM);
    C.Matrix_Zeros();

    for (int m = 0; m < MOV; ++m) {
    // Each Move Takes one cycle
        for (int i = 0; i < DIM; ++i) {
            for (int j = DIM - 1; j >= 0; j--) {
                // incoming A
                int a_in = (j == 0) ?
                           ((m <= DIM-1) ? A.get(m,i) : 0) :
                            pe[i][j-1].ia;
                // MAC
                pe[i][j].ia = a_in;
                pe[i][j].psum = a_in * pe[i][j].weight;
            }
        }
        // print_SystolicArray_I();
        // print_SystolicArray_P();

        // Accu
        int acc[DIM] = {0};
        for (int j = 0; j < DIM; ++j){ 
            for (int i = 0; i < DIM; ++i) {
                acc[j] = acc[j] + pe[i][j].psum;
            }
        }
        
        // Output Matrix C
        for (int i = 0; i < DIM; ++ i){
            for (int j = 0; j < DIM; ++j){
                int acc2spm = ((i + j) == m) ? acc[j] : C.get(i,j);
                // cout<<"["<<i<<","<<j<<"]:-"<<m<<":"<<acc2spm<<endl;
                C.set(i , j, acc2spm);
            }
        }
        // C.print();
    
        last_cycles = last_cycles + 1;
    }
    return C;
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