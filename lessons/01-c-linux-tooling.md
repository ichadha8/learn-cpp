# Lesson 01: Linux C Tooling, Translation, and Debugging

## What This Lesson Teaches

This lesson teaches the workflow you should use for every C assignment in this
repo. C is small as a language, but the development environment around it is not
small. You need to understand what the compiler does, what the linker does, what
the loader does, and how to debug a program after it compiles but behaves
incorrectly.

By the end, you should be able to:

- explain the difference between preprocessing, compiling, assembling, linking,
  and loading;
- recognize common compile-time vs link-time errors;
- build and test with CMake;
- use GDB to inspect a failing C program;
- use Valgrind or sanitizers to find memory bugs;
- keep stdout clean for testable Unix programs.

## Big Picture: Source Code Is Not the Program Yet

When you write `main.c`, you have not written an executable. You have written
one input to a toolchain. The rough path is:

```text
source files
  -> preprocessor
  -> compiler
  -> assembler
  -> linker
  -> executable file
  -> loader at runtime
  -> running process
```

Each stage can fail for different reasons.

### Preprocessor

The preprocessor handles lines that begin with `#`, such as:

```c
#include <stdio.h>
#define BUFFER_SIZE 1024
```

It performs textual inclusion and macro expansion before the compiler sees the
program. That means a missing header is usually found before ordinary type
checking.

Useful command:

```bash
gcc -E src/main.c
```

This prints the preprocessed translation unit. It is noisy, but it helps you
understand that `#include` literally brings declarations into the file being
compiled.

### Compiler

The compiler checks C syntax and types, then turns the translation unit into
assembly or object code.

Examples of compiler errors:

- missing semicolon;
- assigning a pointer to an integer without a cast;
- calling a function before it is declared;
- using a struct field that does not exist.

Useful commands:

```bash
gcc -S src/main.c
gcc -c src/main.c -o main.o
```

### Assembler

The assembler turns assembly into machine-code object files. In normal work you
rarely call it directly, but it is part of the pipeline.

### Linker

The linker combines object files and libraries into an executable. The compiler
can say "this function call is syntactically valid" while the linker later says
"I cannot find the implementation."

Example:

```c
/* main.c */
int helper(void);

int main(void) {
    return helper();
}
```

If no object file defines `helper`, compilation can succeed but linking fails
with an undefined reference.

Useful commands:

```bash
nm build/path/program
ldd build/path/program
objdump -d build/path/program
```

`nm` shows symbols. `ldd` shows shared-library dependencies. `objdump` can show
disassembly.

### Loader

The loader maps the executable and shared libraries into memory, sets up the
initial process state, and starts execution. Runtime errors such as missing
shared libraries happen at this layer.

## CMake in This Repo

This repo uses CMake presets so every assignment has a predictable command
shape:

```bash
cmake --preset hw01
cmake --build --preset hw01
ctest --preset hw01 --output-on-failure
```

What the commands mean:

- `cmake --preset hw01`: configure the build directory for HW01.
- `cmake --build --preset hw01`: compile and link.
- `ctest --preset hw01`: run registered tests.

Common CMake failure categories:

- Header not found: include directory missing from a target.
- Undefined reference: source file or library missing from a target.
- Wrong language standard: target not configured for the feature used.
- Test not found: executable was not added with `add_test`.

## Warnings Are Part of Correctness

This course enables warnings such as:

```text
-Wall -Wextra -Wshadow -Wpedantic -Wconversion -Wformat=2 -Wundef
```

Do not treat warnings as decorative. A warning often means the compiler found a
real bug pattern.

Important examples:

- `-Wshadow`: a local variable hides another variable with the same name.
- `-Wconversion`: a value may change when converted to another type.
- `-Wformat=2`: `printf` format string does not match argument types.
- `-Wundef`: a preprocessor condition references an undefined macro.

## Debugging With GDB

Use GDB when the program compiles but you do not understand what it is doing.

Core workflow:

```bash
gdb ./build/hw01/assignments/hw01-c-puzzle/hw01_tests
```

Inside GDB:

```gdb
break puzzle_place
run
next
step
print row
print *puzzle
backtrace
frame 1
continue
quit
```

The most important command for crashes is:

```gdb
backtrace
```

It shows the call stack: which functions called which functions to reach the
crash.

## Memory Debugging

C lets you create bugs that normal tests may not catch:

- writing one byte past an array;
- using memory after `free`;
- freeing twice;
- forgetting to free;
- reading uninitialized memory.

Use AddressSanitizer:

```bash
cmake --preset hw01 -DCMAKE_C_FLAGS="-fsanitize=address,undefined -g"
cmake --build --preset hw01
ctest --preset hw01 --output-on-failure
```

Use Valgrind:

```bash
valgrind --leak-check=full ./program
```

AddressSanitizer is usually faster and easier for out-of-bounds and use-after-
free bugs. Valgrind is slower but very useful for leak reports.

## Unix Output Discipline

Many systems programs are tested by exact output. That means stdout is not a
place for random debugging chatter.

Use:

- stdout for specified program output;
- stderr for diagnostic messages;
- debug macros that can be disabled;
- logs only when the assignment specifies them.

Why this matters:

```bash
./program | other_program
```

In Unix, stdout is often another program's input. Extra text can break a
pipeline or an autograder.

## Professor-Style Question Patterns

You should be ready for questions like:

- A file compiles but linking fails with an undefined reference. Which stage is
  failing?
- What tool would you use to inspect a segmentation fault?
- What tool would you use to detect a memory leak?
- What is the difference between stdout and stderr?
- Why can a header compile problem be different from a linker problem?

## Practice

1. Configure and build one homework preset.
2. Run the tests and intentionally inspect one failing test with GDB.
3. Add a temporary `fprintf(stderr, ...)` debug line.
4. Remove the debug line.
5. Run a memory checker on the test binary.

## Assignment Connection

Every homework assumes this workflow. Before you start implementing logic, make
sure you can build, run tests, read compiler messages, and launch GDB. If you
cannot do those things, the later systems topics will feel much harder than
they really are.
