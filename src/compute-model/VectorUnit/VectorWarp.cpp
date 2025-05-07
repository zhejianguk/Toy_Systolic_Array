#include "compute/VecUnit/VectorWarp.h"

VectorWarp::VectorWarp(int warp_size, int lane) : warp_size(warp_size), lane(lane) {
    for (int i = 0; i < warp_size; ++i) {
        warp.push_back(new MetaVector(lane));
    }
    init();
}

VectorWarp::~VectorWarp() {
    for (auto mv : warp) {
        delete mv;
    }
}

void VectorWarp::init() {
    for (int i = 0; i < warp_size; ++i) {
        warp[i]->initVectors();
    }
}

void VectorWarp::matrixMultiply() {
    int** result = new int*[warp_size];
    for (int i = 0; i < warp_size; ++i) {
        result[i] = new int[warp_size];
        for (int j = 0; j < warp_size; ++j) {
            result[i][j] = 0;
        }
    }
    
    for (int i = 0; i < warp_size; ++i) {
        for (int j = 0; j < warp_size; ++j) {
            for (int k = 0; k < lane; ++k) {
                result[i][j] += warp[i]->getVector1()[k] * warp[j]->getVector2()[k];
            }
        }
    }
    
    for (int i = 0; i < warp_size; ++i) {
        MetaVector* mv = warp[i];
        int* v1 = mv->getVector1();
        int* v2 = mv->getVector2();
        
        for (int j = 0; j < warp_size; ++j) {
            if (j < lane) {
                v1[j] = result[i][j];
                v2[j] = result[j][i]; 
            }
        }
    }
    
    for (int i = 0; i < warp_size; ++i) {
        delete[] result[i];
    }
    delete[] result;
}

void VectorWarp::print() const {
    for (int i = 0; i < warp_size; ++i) {
        cout << "MetaVector " << i << ":" << endl;
        warp[i]->print();
    }
} 