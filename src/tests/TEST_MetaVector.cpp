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
    cout << "\nTesting vector initialization..." << endl;
    MetaVector mv(4);
    int* v1 = mv.getVector1();
    int* v2 = mv.getVector2();
    
    // Test vector1 (should be 1,2,3,4)
    for (int i = 0; i < 4; i++) {
        assert(v1[i] == i + 1);
    }
    
    // Test vector2 (should be 4,3,2,1)
    for (int i = 0; i < 4; i++) {
        assert(v2[i] == 4 - i);
    }
    cout << "Initialization test passed!" << endl;
}

void test_set_vectors() {
    cout << "\nTesting set vectors..." << endl;
    MetaVector mv(4);
    
    // Create test arrays
    int test_v1[] = {10, 20, 30, 40};
    int test_v2[] = {40, 30, 20, 10};
    
    // Set vectors
    mv.setVector1(test_v1);
    mv.setVector2(test_v2);
    
    // Verify vectors
    int* v1 = mv.getVector1();
    int* v2 = mv.getVector2();
    
    for (int i = 0; i < 4; i++) {
        assert(v1[i] == test_v1[i]);
        assert(v2[i] == test_v2[i]);
    }
    cout << "Set vectors test passed!" << endl;
}

void test_print() {
    cout << "\nTesting print function..." << endl;
    MetaVector mv(3);
    cout << "Printing vectors:" << endl;
    mv.print();
    cout << "Print test completed!" << endl;
}

int main() {
    cout << "Starting MetaVector tests..." << endl;
    
    try {
        test_constructor();
        test_initialization();
        test_set_vectors();
        test_print();
        
        cout << "\nAll tests passed successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
