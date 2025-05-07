#include "compute/VecUnit/MetaVector.h"
#include <cassert>
#include <iostream>

using namespace std;

void test_constructor() {
    cout << "Testing constructor..." << endl;
    MetaVector mv(4);
    assert(mv.getLane() == 4);
    cout << "Constructor test passed!" << endl;
}

void test_initialization() {
    cout << "\nTesting initialization..." << endl;
    MetaVector mv(4);
    int* v1 = mv.getVector1();
    int* v2 = mv.getVector2();
    
    for (int i = 0; i < 4; i++) {
        assert(v1[i] == i + 1);
        assert(v2[i] == 4 - i);
    }
    cout << "Initialization test passed!" << endl;
}

void test_selfMul() {
    cout << "\nTesting selfMul..." << endl;
    MetaVector mv(4);
    
    int* result = mv.selfMul(1);
    assert(result[0] == 2 * 4);
    assert(result[1] == 2 * 3);
    assert(result[2] == 2 * 2);
    assert(result[3] == 2 * 1);
    
    delete[] result;
    cout << "selfMul test passed!" << endl;
}

void test_selfAdd() {
    cout << "\nTesting selfAdd..." << endl;
    MetaVector mv(4);
    
    int* result = mv.selfAdd();
    assert(result[0] == 1 + 4);
    assert(result[1] == 2 + 3);
    assert(result[2] == 3 + 2);
    assert(result[3] == 4 + 1);
    
    delete[] result;
    cout << "selfAdd test passed!" << endl;
}

void test_print() {
    cout << "\nTesting print function..." << endl;
    MetaVector mv(3);
    cout << "Printing MetaVector:" << endl;
    mv.print();
    cout << "Print test completed!" << endl;
}

int main() {
    cout << "Starting MetaVector tests..." << endl;
    
    try {
        test_constructor();
        test_initialization();
        test_selfMul();
        test_selfAdd();
        test_print();
        
        cout << "\nAll tests passed successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
    
    return 0;
} 
