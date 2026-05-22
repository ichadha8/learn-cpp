# Lesson 01: C Linux Tooling

## Mental Model

C gives you direct access to memory and operating-system interfaces. That power
is why C is still used in kernels, embedded systems, networking, storage,
databases, and performance-critical infrastructure. It also means the tools are
part of the language experience.

You are not "done" when the program compiles. In systems C, the baseline is:

- build reproducibly;
- run tests;
- inspect failures;
- check memory;
- explain behavior.

## Toolchain Pieces

The path from source to executable usually looks like this:

1. Preprocessor expands includes and macros.
2. Compiler turns C into assembly or object code.
3. Assembler emits `.o` files.
4. Linker combines object files and libraries.
5. Loader maps the executable into memory at runtime.

Useful commands:

```bash
gcc -E file.c
gcc -S file.c
gcc -c file.c
gcc file.o -o program
ldd ./program
nm ./program
objdump -d ./program
```

You do not need to memorize every option. You do need to know what stage a
failure belongs to.

## CMake Workflow

This repo uses CMake presets:

```bash
cmake --preset hw01
cmake --build --preset hw01
ctest --preset hw01 --output-on-failure
```

CMake configures the build. Ninja or Make builds it. CTest runs the tests.

If the compiler says a header is missing, that is usually a target include path
problem. If the compiler succeeds but linking fails, that is usually a missing
source file or library.

## Debugging With GDB

Build with debug symbols:

```bash
cmake --preset hw01
cmake --build --preset hw01
gdb ./build/hw01/assignments/hw01-c-puzzle/hw01_tests
```

Core commands:

```gdb
break function_name
run
next
step
print variable
backtrace
frame 1
continue
quit
```

Use GDB when:

- a test fails and you do not know which branch ran;
- a program crashes;
- a pointer is wrong;
- a child process exits unexpectedly.

## Memory Tools

AddressSanitizer catches many memory errors quickly:

```bash
cmake --preset hw01 -DCMAKE_C_FLAGS="-fsanitize=address,undefined -g"
cmake --build --preset hw01
ctest --preset hw01 --output-on-failure
```

Valgrind is slower but useful:

```bash
valgrind --leak-check=full ./program
```

Typical memory bugs:

- reading uninitialized memory;
- writing past an array;
- freeing twice;
- forgetting to free;
- returning a pointer to a local variable;
- using memory after free.

## Output Discipline

Many systems assignments grade exact output. Debug output belongs on `stderr`,
behind a debug macro, or in logs that tests do not parse.

In a Unix pipeline, stdout is data. Treat it with respect.

## Practice

1. Build `hw01`.
2. Run its tests.
3. Pick one failing test and inspect it with GDB.
4. Add a temporary debug print to `stderr`.
5. Remove the debug print before committing.

## Interview Angle

You should be able to answer:

- What is the difference between compile errors and link errors?
- How do you debug a segmentation fault?
- How do you check for memory leaks?
- Why does exact stdout matter for command-line tools?
