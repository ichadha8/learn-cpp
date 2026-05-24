add_library(bonus_cpp_warmup
  src/starship_codex.cpp
)
target_include_directories(bonus_cpp_warmup PUBLIC include)
training_set_cpp_warnings(bonus_cpp_warmup)

add_executable(bonus_cpp_warmup_cli
  src/main.cpp
)
target_link_libraries(bonus_cpp_warmup_cli PRIVATE bonus_cpp_warmup)
training_set_cpp_warnings(bonus_cpp_warmup_cli)

if(TRAINING_ENABLE_STUDENT_TESTS)
  add_executable(bonus_cpp_warmup_tests
    tests/test_starship_codex.cpp
  )
  target_link_libraries(bonus_cpp_warmup_tests PRIVATE bonus_cpp_warmup)
  training_set_cpp_warnings(bonus_cpp_warmup_tests)
  add_test(NAME bonus_cpp_warmup_tests COMMAND bonus_cpp_warmup_tests)
endif()
