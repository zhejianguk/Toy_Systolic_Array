#ifndef META_VECTOR_H
#define META_VECTOR_H

#include <iostream>
#include <stdexcept>  

using namespace std;

class MetaVector {
private:
    int** data;  
    int lane;   
    // vector 1 and vector 2 are two pointers to the data array
    int* vector1; 
    int* vector2; 

public:
    MetaVector(int lane);
    ~MetaVector();  

    void initVectors();
    
    int* getVector1() const { return vector1; }
    int* getVector2() const { return vector2; }
    
    void setVector1(int* v1);
    void setVector2(int* v2);
    
    void print() const;
    
    int getLane() const { return lane; }
    
    int* selfMul(int idx) const;
    int* selfAdd() const;
};

#endif // META_VECTOR_H
