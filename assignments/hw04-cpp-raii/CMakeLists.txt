add_library(hw04_raii
  src/raii.cpp
)
target_include_directories(hw04_raii PUBLIC include)
training_set_cpp_warnings(hw04_raii)

if(TRAINING_ENABLE_STUDENT_TESTS)
  add_executable(hw04_tests
    tests/test_raii.cpp
  )
  target_link_libraries(hw04_tests PRIVATE hw04_raii)
  training_set_cpp_warnings(hw04_tests)
  add_test(NAME hw04_tests COMMAND hw04_tests)
endif()
