#include "Top.h"
#include "compute/SystolicArray.h"
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
void SystolicArray::preLoadWeights(const Matrix& B) {
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
                pe[i][j].psum = pe[i][j].ia * pe[i][j].weight;
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

Matrix SystolicArray::runOutputStationary(const Matrix& A, const Matrix& B) {
    const int MOV = 3*DIM - 1;
    const int DIM_ = 2*DIM - 1;

    Matrix C(DIM, DIM);
    C.Matrix_Zeros();

    // TODO: BEFORE EACH COMPUTE, PSUM SHOULD be reset

    // Transpose A and B to the form which is ready to be moved to SAs
    Matrix A_(DIM, DIM_);
    Matrix B_(DIM_, DIM);
    A_.Matrix_Zeros();
    B_.Matrix_Zeros();
    
    /* In RTL, the above procedures is implemented using delay registers
       The consumed cycles has been calcuated in this 2*DIM - 1 */
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM_; ++j) {
            int d = ((j >= i) && (j < DIM + i)) ? A.get(i,j-i) : 0;
            A_.set(i, j, d);
        }
    }

    for (int i = 0; i < DIM_; ++i) {
        for (int j = 0; j < DIM; ++j) {
            int d = ((i >= j) && (i < DIM + j)) ? B.get(i - j, j) : 0;
            B_.set(i, j, d);
        }
    }

    for (int m = 0; m < MOV; ++m) {
    // Each Move Takes one cycle

        for (int i = 0; i < DIM; ++i){
        // Update Weights
            for (int j = DIM - 1; j >= 0; j--){
                // incoming A
                int a_in = (j == 0) ? ((m <= DIM_-1) ? A_.get(i,m) : 0):
                                       pe[i][j-1].ia;
                pe[i][j].ia = a_in;
            }
        }

        for (int j = 0; j < DIM; ++j){
        // Update IAs
            for (int i = DIM - 1; i >= 0; i--){
                int b_in = (i == 0) ? ((m <= DIM_-1) ? B_.get(m,j) : 0):
                                       pe[i-1][j].weight;
                pe[i][j].weight = b_in;
                // Updated PSUM (another loop is not required)
                pe[i][j].psum = pe[i][j].psum + pe[i][j].ia * pe[i][j].weight;
            }
        }
        last_cycles = last_cycles + 1;
    }

    for (int i = 0; i < DIM; ++ i){
        for (int j = 0; j < DIM; ++j){
            C.set(i , j, pe[i][j].psum);
        }
    }
    
    for (int i = 0; i < DIM; ++i){
        for (int j = 0; j < DIM; ++j){
            // Reset PEs
            pe[i][j].weight = 0;
            pe[i][j].ia = 0;
            pe[i][j].psum = 0;
        }
    }

    return C;
}

Matrix SystolicArray::matmul(const Matrix& A, const Matrix& B) {
    verifyDim(A,B);

    Matrix C = Matmul(A, B);

    ++matmul_count;
    return C;
}