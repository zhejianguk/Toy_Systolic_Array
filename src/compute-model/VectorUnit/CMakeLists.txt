add_library(vector_unit
    MetaVector.cpp
    # BuckyBall.cpp
)

target_include_directories(vector_unit PUBLIC
    ${CMAKE_SOURCE_DIR}/include
) 