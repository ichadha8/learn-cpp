add_library(hw03_fuzzer_lite
  src/fuzzer_lite.c
)
target_include_directories(hw03_fuzzer_lite PUBLIC include)
training_set_c_warnings(hw03_fuzzer_lite)

if(TRAINING_ENABLE_STUDENT_TESTS)
  add_executable(hw03_tests
    tests/test_fuzzer_lite.cpp
  )
  target_link_libraries(hw03_tests PRIVATE hw03_fuzzer_lite)
  training_set_cpp_warnings(hw03_tests)
  add_test(NAME hw03_tests COMMAND hw03_tests)
endif()
