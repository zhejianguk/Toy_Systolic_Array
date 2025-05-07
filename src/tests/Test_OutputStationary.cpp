#include <iostream>
#include "Top.h"
#include "compute/SystolicArray/Matrix.h"
#include "compute/SystolicArray/SystolicArray.h"

using namespace std;

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

    

    cout << "Matrix C = A × B:["<<C.rowCount() << "," << C.colCount() <<"]" << endl;


    // Testing WS
    for (int d1 = 0; d1 < DIM1; ++d1) {
        for (int d2 = 0; d2 < DIM2; ++d2) {
            for (int d3 = 0; d3 < DIM3; ++d3) {

                /*** Padding ***/
                int subARows = std::min(DIM_SIZE, MATRIX_A_ROWS - d1 * DIM_SIZE);
                int subACols = std::min(DIM_SIZE, MATRIX_A_COLS - d3 * DIM_SIZE);
                int subBRows = std::min(DIM_SIZE, MATRIX_B_ROWS - d3 * DIM_SIZE);
                int subBCols = std::min(DIM_SIZE, MATRIX_B_COLS - d2 * DIM_SIZE);
                Matrix subA(DIM_SIZE, DIM_SIZE);
                Matrix subB(DIM_SIZE, DIM_SIZE);
                subA.Matrix_Zeros();
                subB.Matrix_Zeros();

                for (int i = 0; i < subARows; ++i)
                    for (int j = 0; j < subACols; ++j) 
                        subA.set(i, j, A.get(d1 * DIM_SIZE + i, d3 * DIM_SIZE + j));
                                        
                for (int i = 0; i < subBRows; ++i) 
                    for (int j = 0; j < subBCols; ++j) 
                        subB.set(i, j, B.get(d3 * DIM_SIZE + i, d2 * DIM_SIZE + j));

                /*** Output Stationary ****/
                Matrix subC = SA.runOutputStationary(subA, subB);
                Matrix subD = SA.matmul(subA, subB); // Reference Results

                for (int i = 0; i < DIM_SIZE; ++i) 
                    for (int j = 0; j < DIM_SIZE; ++j){
                        if (subC.get(i,j) != subD.get(i,j)){
                            cout << "Error: d1=" << d1 <<"d2="<<d2<<"d3="<<d3<<endl;
                            cout << "[" << i <<","<<j<<"]"<<endl;
                            cout << "reference:" << subD.get(i,j)<<endl;
                            cout << "WS:" << subC.get(i,j)<<endl;
                            exit(0);
                        }
                    }
                        

                /*** Writing Back ****/
                int y = d1 * DIM_SIZE;
                int x = d2 * DIM_SIZE;
                for (int i = 0; i < subARows; ++i) 
                    for (int j = 0; j < subBCols; ++j) 
                        C.set(y + i, x + j, C.get(y + i, x + j) + subC.get(i,j));
            }
        }
    }

    // Check if D = C 
    for (int i = 0; i < C.rowCount(); ++ i) 
        for (int j = 0; j < C.colCount(); ++j)
            if (C.get(i,j) != D.get(i,j))
                cout << "Error!" << endl;

    cout << "OS Tested, Cycles:" << SA.getCycles() << endl;
    return 0;
}

