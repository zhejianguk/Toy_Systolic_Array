#ifndef VECTOR_WARP_H
#define VECTOR_WARP_H

#include "compute/VecUnit/MetaVector.h"
#include <vector>

using namespace std;

class VectorWarp {
private:
    vector<MetaVector*> warp;
    int warp_size;
    int lane;

public:
    VectorWarp(int warp_size, int lane);
    ~VectorWarp();

    void init();
    void matrixMultiply();
    void print() const;
    
    int getWarpSize() const { return warp_size; }
    int getLane() const { return lane; }
    MetaVector* getMetaVector(int index) const { return warp[index]; }
};

#endif // VECTOR_WARP_H
