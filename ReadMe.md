Build the simulator:

```
mkdir build
cd build
cmake -G Ninja ..
ninja
```

Run simulation:

```
./build/src/tests/Test_${TEST_NAME}
```

The ${TEST_NAME} can be configured as:
```
1. MatMul
2. WeightStationary
3. OutputStationary
4. RowStationary
5. MetaVector
6. VectorWarp
```
