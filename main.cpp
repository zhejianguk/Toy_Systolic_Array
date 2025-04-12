#include <iostream>
#include "Matrix.h"
#include "SystolicArray.h"

using namespace std;

int MATRIX_A_ROWS = 2048;
int MATRIX_A_COLS = 1024;
int MATRIX_B_ROWS = 1024;
int MATRIX_B_COLS = 512;
int DIM_SIZE = 17;

int main() {
    Matrix A(MATRIX_A_ROWS, MATRIX_A_COLS);   // 3 rows, 8 columns
    Matrix B(MATRIX_B_ROWS, MATRIX_B_COLS);  // 8 rows, 10 columns
    SystolicArray SA(DIM_SIZE);

    // Use overloaded * operator
    Matrix D = A * B;
    Matrix C(A.rowCount(), B.colCount()); 
    C.Matrix_Zeros();

    // Systolic Array
    int DIM1 = (MATRIX_A_ROWS + DIM_SIZE - 1) / DIM_SIZE; 
    int DIM2 = (MATRIX_B_COLS + DIM_SIZE - 1) / DIM_SIZE; 
    int DIM3 = (MATRIX_A_COLS + DIM_SIZE - 1) / DIM_SIZE;

    for (int d1 = 0; d1 < DIM1; ++d1) {
        for (int d2 = 0; d2 < DIM2; ++d2) {
            for (int d3 = 0; d3 < DIM3; ++d3) {

                // These may be smaller than DIM_SIZE if we're at the bottom/right edge
                int subARows = std::min(DIM_SIZE, MATRIX_A_ROWS - d1 * DIM_SIZE);
                int subACols = std::min(DIM_SIZE, MATRIX_A_COLS - d3 * DIM_SIZE);
                int subBRows = std::min(DIM_SIZE, MATRIX_B_ROWS - d3 * DIM_SIZE);
                int subBCols = std::min(DIM_SIZE, MATRIX_B_COLS - d2 * DIM_SIZE);

                // Create sub-matrices (tiles) for A and B
                Matrix subA(DIM_SIZE, DIM_SIZE);
                Matrix subB(DIM_SIZE, DIM_SIZE);
                subA.Matrix_Zeros();
                subB.Matrix_Zeros();


                for (int i = 0; i < subARows; ++i)
                    for (int j = 0; j < subACols; ++j) 
                        // cout << " subA [" << i << j << "] =  A [" << (d1 * DIM_SIZE + i) << (d3 * DIM_SIZE + j) << "] = " << A.get(d1 * DIM_SIZE + i, d3 * DIM_SIZE + j) << endl;
                        subA.set(i, j, A.get(d1 * DIM_SIZE + i, d3 * DIM_SIZE + j));
                                        
                for (int i = 0; i < subBRows; ++i) 
                    for (int j = 0; j < subBCols; ++j) 
                        // cout << " subB [" << i << j << "] =  B [" << (d3 * DIM_SIZE + i) << (d2 * DIM_SIZE + j) << "] = " << B.get(d3 * DIM_SIZE + i, d2 * DIM_SIZE + j) << endl;
                        subB.set(i, j, B.get(d3 * DIM_SIZE + i, d2 * DIM_SIZE + j));

                // Multiply the DIM_SIZE x DIM_SIZE (padded) subA and subB
                Matrix subC = SA.matmul(subA, subB);
               

                // Accumulator
                int y = d1 * DIM_SIZE;
                int x = d2 * DIM_SIZE;
                for (int i = 0; i < subARows; ++i) 
                    for (int j = 0; j < subBCols; ++j) 
                        C.set(y + i, x + j, C.get(y + i, x + j) + subC.get(i,j));
            }
        }
    }
    

    cout << "Matrix C = A × B:["<<C.rowCount() << "," << C.colCount() <<"]" << endl;
    // C.print();
    cout << "Counts for Matmul:" << SA.getMatmulCount() << endl;
    

    // cout << "Matrix D = A × B:" << endl;
    // D.print();


    // Check if D = C 
    for (int i = 0; i < C.rowCount(); ++ i) 
        for (int j = 0; j < C.colCount(); ++j)
            if (C.get(i,j) != D.get(i,j))
                cout << "Error!" << endl;

    return 0;
}

