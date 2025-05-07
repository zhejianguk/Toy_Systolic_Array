# Create computation model library
add_library(computation_model
    Matrix.cpp
    SystolicArray.cpp
)

# Set include directories
target_include_directories(computation_model PUBLIC
    ${CMAKE_SOURCE_DIR}/include
) 