set(TEST_TARGETS
    Test_MatMul
    Test_WeightSationary
    Test_OutputStationary
    Test_RowStationary
    Test_MetaVector
)

foreach(TEST ${TEST_TARGETS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} PRIVATE compute-model)
endforeach() 