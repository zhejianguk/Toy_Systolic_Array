set(TEST_TARGETS
    Test_MatMul
    Test_WeightSationary
    Test_OutputStationary
)

foreach(TEST ${TEST_TARGETS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} PRIVATE computation_model)
endforeach() 