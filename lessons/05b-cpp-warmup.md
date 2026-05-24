# Lesson 05b: C++ Warmup - From C Syntax to C++ Types

## Why This Lesson Exists

The RAII/STL lesson assumes you are already comfortable writing small C++
programs. This bridge lesson is the missing on-ramp: it teaches the C++ syntax,
library habits, and type-design ideas you need before resource ownership gets
serious.

If you know C, the trap is to treat C++ as "C plus a few keywords." That works
for tiny programs and breaks down quickly. Good C++ code leans on stronger
types, constructors, namespaces, references, standard containers, and standard
algorithms. This lesson keeps the scope small, but it asks you to start thinking
like a C++ programmer.

By the end, you should be able to:

- compile a small multi-file C++ project;
- use `std::string`, `std::string_view`, `std::vector`, and `std::optional`;
- pass objects by value, reference, or `const` reference intentionally;
- define `enum class`, `struct`, and a small `class`;
- split declarations into headers and definitions into `.cpp` files;
- write tests for parser and collection logic;
- explain why the starter avoids raw owning pointers.

## Required LearnCpp Reading

Read these pages before attempting the bonus homework. They are short enough to
study in one or two focused sessions, and they map directly onto the code you
will write.

Core program shape:

- LearnCpp 1.5, I/O with `std::cout` and `std::cin`: https://www.learncpp.com/cpp-tutorial/introduction-to-iostream-cout-cin-and-endl/
- LearnCpp 1.3, objects and variables: https://www.learncpp.com/cpp-tutorial/introduction-to-objects-and-variables/
- LearnCpp 2.1, functions: https://www.learncpp.com/cpp-tutorial/introduction-to-functions/
- LearnCpp 2.4, function parameters and arguments: https://www.learncpp.com/cpp-tutorial/introduction-to-function-parameters-and-arguments/

Project structure:

- LearnCpp 2.9, namespaces and naming collisions: https://www.learncpp.com/cpp-tutorial/naming-collisions-and-an-introduction-to-namespaces/
- LearnCpp 2.11, header files: https://www.learncpp.com/cpp-tutorial/header-files/
- LearnCpp 7.2, user-defined namespaces: https://www.learncpp.com/cpp-tutorial/user-defined-namespaces-and-the-scope-resolution-operator/

Strings and parameter passing:

- LearnCpp 5.7, `std::string`: https://www.learncpp.com/cpp-tutorial/introduction-to-stdstring/
- LearnCpp 5.8, `std::string_view`: https://www.learncpp.com/cpp-tutorial/introduction-to-stdstring_view/
- LearnCpp 12.6, pass by `const` lvalue reference: https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/
- LearnCpp 12.7, raw pointers in modern C++: https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/

Program-defined types:

- LearnCpp 13.1, user-defined types: https://www.learncpp.com/cpp-tutorial/introduction-to-program-defined-user-defined-types/
- LearnCpp 13.6, scoped enumerations: https://www.learncpp.com/cpp-tutorial/scoped-enumerations-enum-classes/
- LearnCpp 13.7, structs and member selection: https://www.learncpp.com/cpp-tutorial/introduction-to-structs-members-and-member-selection/
- LearnCpp 13.8, aggregate initialization: https://www.learncpp.com/cpp-tutorial/struct-aggregate-initialization/
- LearnCpp 14.2, classes: https://www.learncpp.com/cpp-tutorial/introduction-to-classes/
- LearnCpp 14.3, member functions: https://www.learncpp.com/cpp-tutorial/member-functions/
- LearnCpp 14.5, public and private members: https://www.learncpp.com/cpp-tutorial/public-and-private-members-and-access-specifiers/
- LearnCpp 14.9, constructors: https://www.learncpp.com/cpp-tutorial/introduction-to-constructors/

Containers and algorithms:

- LearnCpp 16.1, containers and arrays: https://www.learncpp.com/cpp-tutorial/introduction-to-containers-and-arrays/
- LearnCpp 16.2, `std::vector`: https://www.learncpp.com/cpp-tutorial/introduction-to-stdvector-and-list-constructors/
- LearnCpp 18.2, iterators: https://www.learncpp.com/cpp-tutorial/introduction-to-iterators/
- LearnCpp 18.3, standard library algorithms: https://www.learncpp.com/cpp-tutorial/introduction-to-standard-library-algorithms/

Optional video reinforcement:

- CppCon 2020 Back to Basics track: https://cppcon.org/b2b2020/
- Herb Sutter, "Back to the Basics! Essentials of Modern C++ Style": https://isocpp.org/blog/2015/02/cppcon-2014-back-to-the-basics-essentials-of-modern-c-style-herb-sutter

## C++ Is Not Just C With `class`

C gives you direct control and a small language surface. C++ keeps low-level
control but adds a larger type system and a large standard library. That means
you should reach for library types earlier.

C habit:

```c
char name[64];
int count;
```

C++ habit:

```cpp
std::string name{};
int count{};
```

The braces matter. In modern C++, prefer brace initialization because it makes
uninitialized values less likely and catches some narrowing conversions.

```cpp
int x{};       // value-initialized to 0
int y{42};     // initialized to 42
double z{3.5}; // initialized to 3.5
```

This does not mean braces solve every problem. For `std::vector<int>`, there is
a difference between `std::vector<int> values(10)` and
`std::vector<int> values{10}`. The first means ten zeroes; the second means one
element whose value is ten. That is one reason this lesson includes explicit
vector practice.

## Translation Units, Headers, and Namespaces

C++ still compiles one source file at a time. A `.cpp` file is a translation
unit. Headers are copied into translation units by `#include`.

The usual shape is:

```text
include/starship_codex.hpp   declarations
src/starship_codex.cpp       definitions
tests/test_starship_codex.cpp tests
```

Headers should expose the public interface. Source files should contain the
implementation details.

Do not put `using namespace std;` in a header. It forces every file that includes
the header to import the entire `std` namespace, which can create collisions and
surprising compiler errors. Prefer qualified names:

```cpp
std::string name{};
std::vector<int> values{};
```

For your own code, use a course namespace:

```cpp
namespace training {

struct Ship {
    std::string name{};
};

} // namespace training
```

This keeps names like `Ship`, `Codex`, or `parse_ship_line` from colliding with
other libraries or future assignments.

## Strings and Views

Use `std::string` when your object owns text.

```cpp
struct Ship {
    std::string name{};
};
```

Use `std::string_view` when a function only needs to read text and does not need
to store it.

```cpp
bool is_valid_ship_name(std::string_view name);
```

`std::string_view` is cheap because it views existing characters rather than
owning a new copy. That also means you must not store a `string_view` if the text
it views might disappear. In this warmup, parse functions may accept
`std::string_view`, but the `Ship` struct stores a real `std::string`.

## Function Parameters

C programmers often pass pointers because that is the only way to avoid copying
or to mutate caller-owned data. C++ gives you more choices.

Use pass by value for small, cheap values:

```cpp
std::string to_string(ShipClass ship_class);
```

Use `const T&` for read-only access to a larger owned object:

```cpp
std::string format_ship(const Ship& ship);
```

Use `T&` when mutation is part of the contract:

```cpp
void normalize_name(std::string& name);
```

Use a raw pointer when `nullptr` is meaningful or when you are working with C
APIs. This warmup does not need raw owning pointers.

## `enum class`

Plain C enums leak their enumerator names into the surrounding scope and convert
to integers too freely. C++ `enum class` is scoped and type-safe:

```cpp
enum class ShipClass {
    Scout,
    Freighter,
    Cruiser,
    Carrier,
    Unknown
};
```

To use a value, qualify it:

```cpp
ShipClass kind{ShipClass::Scout};
```

That extra prefix is useful. It keeps `Scout` from becoming a loose global name,
and it prevents accidental comparisons between unrelated enums.

## Structs, Classes, and Invariants

In C++, both `struct` and `class` can have data and functions. The main
difference is default access:

- `struct` members are public by default;
- `class` members are private by default.

Use a `struct` when the type is mostly plain data:

```cpp
struct Ship {
    std::string name{};
    ShipClass ship_class{ShipClass::Unknown};
    int rating{0};
    bool active{false};
};
```

Use a `class` when the type protects an invariant:

```cpp
class Codex {
public:
    bool add(Ship ship);
    std::size_t size() const noexcept;

private:
    std::vector<Ship> ships_{};
};
```

The `Codex` class owns a collection. It can prevent duplicate names, control how
ships are added, and expose read-only queries without letting callers mutate the
internal vector directly.

## `std::vector`

`std::vector<T>` is the default growable array in C++. It owns contiguous memory,
tracks its length, and cleans itself up.

```cpp
std::vector<Ship> ships{};
ships.push_back(Ship{"Aurora", ShipClass::Scout, 8, true});
```

Prefer range-based loops for simple traversal:

```cpp
for (const Ship& ship : ships) {
    // inspect ship without copying it
}
```

Use indexes only when the index is part of the logic.

## `std::optional`

Many C functions use sentinel values: `NULL`, `-1`, empty strings, or output
parameters. C++ often uses `std::optional<T>` to say "this function may not
produce a value."

```cpp
std::optional<Ship> parse_ship_line(std::string_view line);
```

If parsing fails, return `std::nullopt`. If parsing succeeds, return a `Ship`.
The caller must check whether a value exists:

```cpp
auto parsed{parse_ship_line("Aurora|scout|8|active")};
if (parsed.has_value()) {
    Ship ship{*parsed};
}
```

## Standard Algorithms

Loops are fine, but C++ also gives you algorithms that communicate intent:

```cpp
auto found = std::find_if(ships.begin(), ships.end(),
    [&](const Ship& ship) {
        return ship.name == target;
    });
```

You do not need to use algorithms everywhere. For this warmup, write a mix:
simple loops where they are clearest, and one or two algorithms where they make
the code easier to read.

## Error Handling in the Warmup

This module does not use exceptions for assignment-level validation. Instead:

- parser functions return `std::optional`;
- functions with invalid input return `false` or an empty vector;
- tests check exact behavior.

Later lessons will revisit exception safety and RAII. For now, focus on clear
contracts and predictable return values.

## Common C-to-C++ Mistakes

- Writing `malloc` and `free` in ordinary C++ code instead of using values and
  containers.
- Passing every object by pointer.
- Returning references to local variables.
- Storing `std::string_view` inside long-lived objects.
- Putting `using namespace std;` in headers.
- Exposing class internals just because tests need to inspect behavior.
- Forgetting `const` on methods that only inspect state.
- Assuming `std::vector<int> v{10}` means ten elements.

## Practice Before the Homework

Write these tiny functions in a scratch file before opening the starter:

1. A function that accepts `std::string_view` and returns whether it is empty.
2. A function that converts `"yes"` or `"no"` into `std::optional<bool>`.
3. A `struct` with a `std::string`, an `int`, and a `bool`.
4. A `std::vector` of that struct.
5. A loop that finds the highest integer field in the vector.

If those feel comfortable, start the bonus homework. If not, read the linked
LearnCpp pages again and type out the examples by hand.

## Assignment Connection

The bonus assignment is `assignments/bonus-cpp-warmup`. It is intentionally
lighter than HW04, but it trains the habits HW04 expects:

- headers and source files;
- namespace hygiene;
- `enum class`;
- value structs;
- `std::string` and `std::string_view`;
- `std::vector`;
- parser tests;
- a small class that protects its internal state.

Treat it as the C++ equivalent of learning to walk before sprinting into RAII,
file descriptors, move semantics, and servers.
