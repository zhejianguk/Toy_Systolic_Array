Build the simulator:

```
mkdir build
cd build
cmake -G Ninja ..
ninja
```

Run simulation:

```
./build/src/tests/Test_MatMul
./build/src/tests/Test_WeightSationary
./build/src/tests/Test_OutputStationary
```

The ${TEST_NAME} can be configured as:
```
1. MatMul
2. WeightStationary
3. OutputStationary
```
