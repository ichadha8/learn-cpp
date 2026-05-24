find_package(Threads REQUIRED)

add_library(pulsekv_core
  src/store.cpp
  src/protocol.cpp
  src/server.cpp
)
target_include_directories(pulsekv_core PUBLIC include)
target_link_libraries(pulsekv_core PUBLIC Threads::Threads)
training_set_cpp_warnings(pulsekv_core)

add_executable(pulsekv
  src/main.cpp
)
target_link_libraries(pulsekv PRIVATE pulsekv_core)
training_set_cpp_warnings(pulsekv)

if(TRAINING_ENABLE_STUDENT_TESTS)
  add_executable(pulsekv_tests
    tests/test_pulsekv.cpp
  )
  target_link_libraries(pulsekv_tests PRIVATE pulsekv_core)
  training_set_cpp_warnings(pulsekv_tests)
  add_test(NAME pulsekv_tests COMMAND pulsekv_tests)
endif()
