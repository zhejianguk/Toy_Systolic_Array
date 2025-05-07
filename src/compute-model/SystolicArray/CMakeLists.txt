add_library(systolic_array
    Matrix.cpp
    SystolicArray.cpp
)

target_include_directories(systolic_array PUBLIC
    ${CMAKE_SOURCE_DIR}/include
) 