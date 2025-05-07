#include "compute/VecUnit/VectorWarp.h"
#include <cassert>
#include <iostream>
#include <chrono>

using namespace std;

void test_constructor() {
    cout << "Testing constructor..." << endl;
    VectorWarp warp(16, 16);
    assert(warp.getWarpSize() == 16);
    assert(warp.getLane() == 16);
    cout << "Constructor test passed!" << endl;
}

void test_initialization() {
    cout << "\nTesting initialization..." << endl;
    VectorWarp warp(16, 16);
    
    for (int i = 0; i < warp.getWarpSize(); ++i) {
        MetaVector* mv = warp.getMetaVector(i);
        int* v1 = mv->getVector1();
        int* v2 = mv->getVector2();
        
        for (int j = 0; j < warp.getLane(); ++j) {
            assert(v1[j] == j + 1);
            assert(v2[j] == warp.getLane() - j);
        }
    }
    cout << "Initialization test passed!" << endl;
}

void test_matrix_multiply() {
    cout << "\nTesting matrix multiplication with 16x16 matrix..." << endl;
    VectorWarp warp(16, 16);
    
    cout << "First few rows and columns before multiplication:" << endl;
    for (int i = 0; i < 3; ++i) {
        MetaVector* mv = warp.getMetaVector(i);
        cout << "MetaVector " << i << ": [";
        for (int j = 0; j < 3; ++j) {
            cout << mv->getVector1()[j] << " ";
        }
        cout << "...]" << endl;
    }
    cout << "..." << endl;
    
    auto start = chrono::high_resolution_clock::now();
    warp.matrixMultiply();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    cout << "\nAfter multiplication (took " << duration << " ms):" << endl;
    cout << "First few rows and columns of result:" << endl;
    for (int i = 0; i < 3; ++i) {
        MetaVector* mv = warp.getMetaVector(i);
        cout << "MetaVector " << i << " (Row " << i << "): [";
        for (int j = 0; j < 3; ++j) {
            cout << mv->getVector1()[j] << " ";
        }
        cout << "...]" << endl;
    }
    cout << "..." << endl;
    
    cout << "Matrix multiplication test completed!" << endl;
}

// 简单地验证一个元素的计算是否正确
void verify_single_element() {
    cout << "\nVerifying a single element calculation..." << endl;
    VectorWarp warp(16, 16);
    warp.matrixMultiply();
    
    MetaVector* mv = warp.getMetaVector(0);
    int* v1 = mv->getVector1();
    
    int expected = 0;
    for (int k = 0; k < 16; ++k) {
        expected += (k + 1) * (16 - k);
    }
    
    cout << "Expected result[0][0] = " << expected << endl;
    cout << "Actual result[0][0] = " << v1[0] << endl;
    assert(v1[0] == expected);
    
    cout << "Single element verification passed!" << endl;
}

int main() {
    cout << "Starting VectorWarp tests with 16x16 matrices..." << endl;
    
    try {
        test_constructor();
        test_initialization();
        test_matrix_multiply();
        verify_single_element();
        
        cout << "\nAll tests passed successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
    
    return 0;
} 
