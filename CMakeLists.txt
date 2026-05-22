cmake_minimum_required(VERSION 3.22)

project(c_cpp_systems_training LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TRAINING_ASSIGNMENT "all" CACHE STRING "Assignment to configure: all, hw01, hw02, hw03, hw04, hw05, capstone-c, capstone-cpp")
option(TRAINING_ENABLE_STUDENT_TESTS "Build and register assignment tests" OFF)

enable_testing()

include(cmake/TrainingWarnings.cmake)

function(training_add_if_matches name path)
  if(TRAINING_ASSIGNMENT STREQUAL "all" OR TRAINING_ASSIGNMENT STREQUAL "${name}")
    add_subdirectory("${path}")
  endif()
endfunction()

training_add_if_matches(hw01 assignments/hw01-c-puzzle)
training_add_if_matches(hw02 assignments/hw02-unix-runner)
training_add_if_matches(hw03 assignments/hw03-fuzzer-lite)
training_add_if_matches(hw04 assignments/hw04-cpp-raii)
training_add_if_matches(hw05 assignments/hw05-concurrent-server)
training_add_if_matches(capstone-c capstones/c-fuzzlab)
training_add_if_matches(capstone-cpp capstones/cpp-pulsekv)
