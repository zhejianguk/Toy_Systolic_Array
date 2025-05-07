add_library(vector_unit
    MetaVector.cpp
    VectorWarp.cpp
)

target_include_directories(vector_unit PUBLIC
    ${CMAKE_SOURCE_DIR}/include
) 