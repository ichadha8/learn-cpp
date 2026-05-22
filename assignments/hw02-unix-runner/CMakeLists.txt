add_library(hw02_runner
  src/runner.c
)
target_include_directories(hw02_runner PUBLIC include)
training_set_c_warnings(hw02_runner)

if(TRAINING_ENABLE_STUDENT_TESTS)
  add_executable(hw02_tests
    tests/test_runner.cpp
  )
  target_link_libraries(hw02_tests PRIVATE hw02_runner)
  training_set_cpp_warnings(hw02_tests)
  add_test(NAME hw02_tests COMMAND hw02_tests)
endif()
