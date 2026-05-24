add_library(hw01_puzzle
  src/puzzle.c
)
target_include_directories(hw01_puzzle PUBLIC include)
training_set_c_warnings(hw01_puzzle)

if(TRAINING_ENABLE_STUDENT_TESTS)
  add_executable(hw01_tests
    tests/test_puzzle.cpp
  )
  target_link_libraries(hw01_tests PRIVATE hw01_puzzle)
  training_set_cpp_warnings(hw01_tests)
  add_test(NAME hw01_tests COMMAND hw01_tests)
endif()
