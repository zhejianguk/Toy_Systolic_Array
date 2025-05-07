#include "compute/VecUnit/MetaVector.h"

MetaVector::MetaVector(int lane) : lane(lane) {
    data = new int*[2];
    data[0] = new int[lane];
    data[1] = new int[lane];
    
    vector1 = data[0];
    vector2 = data[1];
    
    initVectors();
}

MetaVector::~MetaVector() {
    delete[] data[0];
    delete[] data[1];
    delete[] data;
}

void MetaVector::initVectors() {
    for (int i = 0; i < lane; ++i) {
        vector1[i] = i + 1;
        vector2[i] = lane - i;
    }
}

void MetaVector::setVector1(int* v1) {
    for (int i = 0; i < lane; ++i) {
        vector1[i] = v1[i];
    }
}

void MetaVector::setVector2(int* v2) {
    for (int i = 0; i < lane; ++i) {
        vector2[i] = v2[i];
    }
}

void MetaVector::print() const {
    cout << "Vector 1: ";
    for (int i = 0; i < lane; ++i) {
        cout << vector1[i] << " ";
    }
    cout << endl;
    
    cout << "Vector 2: ";
    for (int i = 0; i < lane; ++i) {
        cout << vector2[i] << " ";
    }
    cout << endl;
}
