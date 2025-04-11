#include <iostream>
using namespace std;

class Matrix {
private:
    int** data;
    int rows, cols;

public:
    // Constructor with auto-initialization (sequential numbers)
    Matrix(int r, int c) : rows(r), cols(c) {
        data = new int*[rows];
        for (int i = 0; i < rows; ++i)
            data[i] = new int[cols];

        // Initialize with increasing numbers
        int val = 1;
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] = val++;
    }

    // Copy constructor (deep copy)
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = new int*[rows];
        for (int i = 0; i < rows; ++i) {
            data[i] = new int[cols];
            for (int j = 0; j < cols; ++j)
                data[i][j] = other.data[i][j];
        }
    }

    void Matrix_Zeros() {
        for (int i = 0; i < rows; ++i) 
            for (int j = 0; j < cols; ++j)
                data[i][j] = 0;
    }

    // Destructor
    ~Matrix() {
        for (int i = 0; i < rows; ++i)
            delete[] data[i];
        delete[] data;
    }

    // Print matrix
    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j)
                cout << data[i][j] << "\t";
            cout << endl;
        }
    }

    // Accessor
    int get(int i, int j) const { return data[i][j]; }

    // Mutator
    void set(int i, int j, int val) { data[i][j] = val; }

    // Get dimensions
    int rowCount() const { return rows; }
    int colCount() const { return cols; }

    // Static multiplication function (still useful)
    static Matrix multiply(const Matrix& A, const Matrix& B) {
        return A * B;
    }

    // Operator overloading for A * B
    Matrix operator*(const Matrix& B) const {
        if (cols != B.rows) {
            throw invalid_argument("Matrix multiplication error: A's columns must equal B's rows.");
        }

        Matrix result(rows, B.cols);
        result.Matrix_Zeros();

        // Matrix multiplication
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < B.cols; ++j)
                for (int k = 0; k < cols; ++k)
                    result.set(i, j, result.get(i, j) + data[i][k] * B.get(k, j));

        return result;
    }

    friend Matrix Matmul (const Matrix &A, const Matrix &B);
};

Matrix Matmul (const Matrix &A, const Matrix &B) {
    if (A.cols != B.rows) {
        throw invalid_argument("Matrix multiplication error: A's columns must equal B's rows."); 
    }

    Matrix C(A.rows, B.cols);
    C.Matrix_Zeros();

    // Matrix multiplication
    for (int i = 0; i < A.rows; ++ i)
        for (int j = 0; j < B.cols; ++ j)
            for (int k = 0; k < A.cols; ++k)
                // C[i][j] += C[i][j] + A[i][k] * B[k][j]
                C.set(i, j, C.get(i,j) + A.get(i, k) * B.get(k, j));
    return C;
}


class SystolicArray {
private:
    int DIM;
    int matmul_count;

public:
    // Constructor: takes dimension size
    SystolicArray(int dim) : DIM(dim), matmul_count(0) {}

    // Getter for matmul count
    int getMatmulCount() const {
        return matmul_count;
    }

    // Perform matrix multiplication
    Matrix matmul(const Matrix& A, const Matrix& B) {
        if (A.rowCount() != DIM || A.colCount() != DIM ||
            B.rowCount() != DIM || B.colCount() != DIM) {
            throw invalid_argument("SystolicArray only supports DIM x DIM square matrices.");
        }

        // Simulate systolic array time steps
        Matrix C = Matmul(A, B);

        // Increment computation count
        ++matmul_count;
        return C;
    }
};

int MATRIX_A_ROWS = 512;
int MATRIX_A_COLS = 1024;
int MATRIX_B_ROWS = 1024;
int MATRIX_B_COLS = 256;
int DIM_SIZE = 32;

int main() {
    Matrix A(MATRIX_A_ROWS, MATRIX_A_COLS);   // 3 rows, 8 columns
    Matrix B(MATRIX_B_ROWS, MATRIX_B_COLS);  // 8 rows, 10 columns
    SystolicArray SA(DIM_SIZE);

    // cout << "Matrix A:" << endl;
    // A.print();

    // cout << "Matrix B:" << endl;
    // B.print();

    // Use overloaded * operator
    Matrix D = A * B;
    Matrix C(A.rowCount(), B.colCount()); 
    C.Matrix_Zeros();

    // Systolic Array
    int DIM1 = MATRIX_A_ROWS/DIM_SIZE;
    int DIM2 = MATRIX_B_COLS/DIM_SIZE;
    int DIM3 = MATRIX_A_COLS/DIM_SIZE;

    for (int d1 = 0; d1 < DIM1; ++d1) {
        for (int d2 = 0; d2 < DIM2; ++d2) {
            for (int d3 = 0; d3 < DIM3; ++d3) {
                // Create sub-matrices (tiles) for A and B
                Matrix subA(DIM_SIZE, DIM_SIZE);
                Matrix subB(DIM_SIZE, DIM_SIZE);

                for (int i = 0; i < DIM_SIZE; ++i)
                    for (int j = 0; j < DIM_SIZE; ++j) 
                        // cout << " subA [" << i << j << "] =  A [" << (d1 * DIM_SIZE + i) << (d3 * DIM_SIZE + j) << "] = " << A.get(d1 * DIM_SIZE + i, d3 * DIM_SIZE + j) << endl;
                        subA.set(i, j, A.get(d1 * DIM_SIZE + i, d3 * DIM_SIZE + j));
                                        
                for (int i = 0; i < DIM_SIZE; ++i) 
                    for (int j = 0; j < DIM_SIZE; ++j) 
                        // cout << " subB [" << i << j << "] =  B [" << (d3 * DIM_SIZE + i) << (d2 * DIM_SIZE + j) << "] = " << B.get(d3 * DIM_SIZE + i, d2 * DIM_SIZE + j) << endl;
                        subB.set(i, j, B.get(d3 * DIM_SIZE + i, d2 * DIM_SIZE + j));

                // Systolic Array
                Matrix subC = SA.matmul(subA, subB);

                // Accumulator
                int y = d1 * DIM_SIZE;
                int x = d2 * DIM_SIZE;
                for (int i = 0; i < DIM_SIZE; ++i) 
                    for (int j = 0; j < DIM_SIZE; ++j) 
                        C.set(y + i, x + j, C.get(y + i, x + j) + subC.get(i,j));
            }
        }
    }
    

    cout << "Matrix C = A × B:["<<C.rowCount() << "," << C.colCount() <<"]" << endl;
    // C.print();
    cout << "Cycles for computation:" << SA.getMatmulCount() << endl;
    

    // cout << "Matrix D = A × B:" << endl;
    // D.print();


    // Check if D = C 
    for (int i = 0; i < C.rowCount(); ++ i) 
        for (int j = 0; j < C.colCount(); ++j)
            if (C.get(i,j) != D.get(i,j))
                cout << "Error!" << endl;

    return 0;
}

