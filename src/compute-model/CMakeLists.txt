add_library(compute-model INTERFACE)

add_subdirectory(SystolicArray)
add_subdirectory(VectorUnit)

target_link_libraries(compute-model INTERFACE 
  systolic_array 
  vector_unit
  )
