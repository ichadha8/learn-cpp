find_package(Threads REQUIRED)

add_library(hw05_server_core
  src/server_core.c
)
target_include_directories(hw05_server_core PUBLIC include)
target_link_libraries(hw05_server_core PUBLIC Threads::Threads)
training_set_c_warnings(hw05_server_core)

if(TRAINING_ENABLE_STUDENT_TESTS)
  add_executable(hw05_tests
    tests/test_server_core.cpp
  )
  target_link_libraries(hw05_tests PRIVATE hw05_server_core)
  training_set_cpp_warnings(hw05_tests)
  add_test(NAME hw05_tests COMMAND hw05_tests)
endif()
