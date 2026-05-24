add_library(fuzzlab_core
  src/mutate.c
  src/runner.c
  src/campaign.c
)
target_include_directories(fuzzlab_core PUBLIC include)
training_set_c_warnings(fuzzlab_core)

add_executable(fuzzlab
  src/main.c
)
target_link_libraries(fuzzlab PRIVATE fuzzlab_core)
training_set_c_warnings(fuzzlab)

add_executable(fuzzlab_crashy
  targets/crashy.c
)
training_set_c_warnings(fuzzlab_crashy)

if(TRAINING_ENABLE_STUDENT_TESTS)
  add_executable(fuzzlab_tests
    tests/test_fuzzlab.cpp
  )
  target_link_libraries(fuzzlab_tests PRIVATE fuzzlab_core)
  training_set_cpp_warnings(fuzzlab_tests)
  add_test(NAME fuzzlab_tests COMMAND fuzzlab_tests)
endif()
