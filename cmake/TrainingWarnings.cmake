function(training_set_c_warnings target)
  target_compile_options(${target} PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wpedantic
    -Wconversion
    -Wformat=2
    -Wundef
  )
endfunction()

function(training_set_cpp_warnings target)
  target_compile_options(${target} PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wpedantic
    -Wconversion
    -Wformat=2
    -Wundef
  )
endfunction()
