# Bonus Homework: C++ Warmup - Starship Codex
#### C/C++ Systems Training

Readme version 1.0

## Due Date

This is a bonus bridge assignment. Do it before `hw04-cpp-raii` if either of
you feels shaky with C++ syntax, headers, classes, `std::vector`, or
`std::optional`.

**Read the entire document before you start.**

## Introduction

In this assignment, you will write a small C++ library called **Starship Codex**.
The codex stores records for fictional ships, parses compact text records,
formats records for display, and supports basic collection queries.

This is intentionally easier than the RAII homework. There are no file
descriptors, no sockets, no threads, and no manual heap ownership. The goal is
to build fluency with everyday C++:

- `std::string` for owned text;
- `std::string_view` for read-only input text;
- `std::vector` for growable arrays;
- `std::optional` for parse failure and missing records;
- `enum class` for type-safe categories;
- `struct` for simple data records;
- `class` for private state and public operations;
- headers, source files, namespaces, and unit tests.

This is a warmup, but do not treat it as throwaway. These are the exact habits
that make the later systems projects easier.

## Required Reading

Read `lessons/05b-cpp-warmup.md` first.

Minimum LearnCpp pages:

- I/O: https://www.learncpp.com/cpp-tutorial/introduction-to-iostream-cout-cin-and-endl/
- Functions: https://www.learncpp.com/cpp-tutorial/introduction-to-functions/
- Headers: https://www.learncpp.com/cpp-tutorial/header-files/
- Namespaces: https://www.learncpp.com/cpp-tutorial/naming-collisions-and-an-introduction-to-namespaces/
- `std::string`: https://www.learncpp.com/cpp-tutorial/introduction-to-stdstring/
- `std::string_view`: https://www.learncpp.com/cpp-tutorial/introduction-to-stdstring_view/
- Pass by `const` reference: https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/
- Scoped enums: https://www.learncpp.com/cpp-tutorial/scoped-enumerations-enum-classes/
- Structs: https://www.learncpp.com/cpp-tutorial/introduction-to-structs-members-and-member-selection/
- Classes: https://www.learncpp.com/cpp-tutorial/introduction-to-classes/
- Member functions: https://www.learncpp.com/cpp-tutorial/member-functions/
- Public/private members: https://www.learncpp.com/cpp-tutorial/public-and-private-members-and-access-specifiers/
- Constructors: https://www.learncpp.com/cpp-tutorial/introduction-to-constructors/
- `std::vector`: https://www.learncpp.com/cpp-tutorial/introduction-to-stdvector-and-list-constructors/
- Algorithms: https://www.learncpp.com/cpp-tutorial/introduction-to-standard-library-algorithms/

Optional video reinforcement:

- CppCon 2020 Back to Basics track: https://cppcon.org/b2b2020/
- Herb Sutter, "Essentials of Modern C++ Style": https://isocpp.org/blog/2015/02/cppcon-2014-back-to-the-basics-essentials-of-modern-c-style-herb-sutter

## Getting Started

From the repository root:

```bash
cmake --preset bonus-cpp-warmup
cmake --build --preset bonus-cpp-warmup
ctest --preset bonus-cpp-warmup --output-on-failure
```

At first, the tests are expected to fail. The starter compiles, but most
functions in `src/starship_codex.cpp` are TODO stubs.

The assignment files are:

```text
assignments/bonus-cpp-warmup
|-- CMakeLists.txt
|-- include
|   `-- starship_codex.hpp
|-- src
|   |-- main.cpp
|   `-- starship_codex.cpp
`-- tests
    `-- test_starship_codex.cpp
```

You should only need to edit:

- `src/starship_codex.cpp`;
- optionally `src/main.cpp` if you want to build a nicer CLI after the tests
  pass;
- optionally add your own extra tests.

Do not change function names or signatures in the public header unless you also
update all tests and docs. The point of the header is to give you a stable API
to implement.

## Project Model

The public header defines a `training` namespace. All assignment symbols live
inside it.

### `ShipClass`

```cpp
enum class ShipClass {
    Scout,
    Freighter,
    Cruiser,
    Carrier,
    Unknown
};
```

Use `ShipClass` for the ship category. Do not represent categories as raw
strings throughout the code. Parse strings at the boundary, then store the
typed value.

Accepted input spellings:

| Input text | Enum value |
|:--|:--|
| `scout` | `ShipClass::Scout` |
| `freighter` | `ShipClass::Freighter` |
| `cruiser` | `ShipClass::Cruiser` |
| `carrier` | `ShipClass::Carrier` |
| `unknown` | `ShipClass::Unknown` |

The parser should be lowercase-only for this warmup. `"Scout"` should be
rejected. Keeping this strict makes tests and debugging simpler.

### `Ship`

```cpp
struct Ship {
    std::string name{};
    ShipClass ship_class{ShipClass::Unknown};
    int rating{0};
    bool active{false};
};
```

`Ship` is a plain value type. It owns its `name` string, stores a typed class,
stores an integer `rating`, and stores whether the ship is currently active.

Valid ship records must obey these rules:

- `name` must be non-empty;
- `name` may contain letters, digits, spaces, underscores, and hyphens;
- `name` may not start or end with a space;
- `ship_class` must be one of the accepted class strings;
- `rating` must be an integer from 1 through 10 for most ships;
- `rating` may be 11 through 20 only for `cruiser` or `carrier`;
- `active` must be parsed from `active`, `inactive`, `yes`, `no`, `true`, or
  `false`.

### Text Record Format

The parser reads one compact record:

```text
name|class|rating|status
```

Example:

```text
Aurora|scout|8|active
```

This should parse to:

```cpp
Ship{
    "Aurora",
    ShipClass::Scout,
    8,
    true
}
```

Malformed records return `std::nullopt`. Do not partially initialize an output
parameter. Do not print error messages from parser functions.

## Required Functions

Implement every function declared in `include/starship_codex.hpp`.

### `std::string to_string(ShipClass ship_class)`

Returns the lowercase spelling of a ship class:

```text
scout
freighter
cruiser
carrier
unknown
```

This function should never fail. If the enum value is somehow outside the known
set, return `unknown`.

Hint: use a `switch`.

### `std::optional<ShipClass> ship_class_from_string(std::string_view text)`

Converts lowercase text into a `ShipClass`.

Return `std::nullopt` if the text is not one of the accepted spellings.

Hint: compare `text` directly with string literals.

### `bool is_valid_ship_name(std::string_view name)`

Returns whether a ship name follows the name rules.

A valid name:

- is not empty;
- does not start or end with a space;
- contains only letters, digits, spaces, underscores, and hyphens.

Hint: use a range-based loop over the characters. `std::isalnum` is allowed,
but remember to cast the character to `unsigned char` before passing it to
`std::isalnum`.

### `std::optional<Ship> parse_ship_line(std::string_view line)`

Parses a full record of the form:

```text
name|class|rating|status
```

Return `std::nullopt` when:

- there are not exactly four fields;
- the name is invalid;
- the class is invalid;
- the rating is not a clean base-10 integer;
- the rating is out of range for the class;
- the status is invalid.

Implementation hints:

- You can split manually using `find('|')` and `substr`.
- Since `std::string_view` does not own text, copy the final name into the
  `Ship` as a `std::string`.
- For rating, use `std::from_chars` from `<charconv>` if you want the cleanest
  no-exception parser.
- Reject `"8abc"`, `"abc"`, empty rating text, negative ratings, and ratings
  with leading plus signs.

### `std::string format_ship(const Ship& ship)`

Formats a ship exactly as:

```text
Aurora [scout] rating=8 active
```

For inactive ships:

```text
Borealis [freighter] rating=4 inactive
```

Hint: `std::ostringstream` is fine here.

### `int total_rating(const std::vector<Ship>& ships)`

Returns the sum of all ratings. Empty input returns `0`.

### `double average_rating(const std::vector<Ship>& ships)`

Returns the average rating. Empty input returns `0.0`.

Make sure this uses floating-point division, not integer division.

### `std::optional<Ship> strongest_ship(const std::vector<Ship>& ships)`

Returns the ship with the highest rating.

Tie-break rule:

- if multiple ships share the highest rating, return the one that appears first
  in the vector.

Empty input returns `std::nullopt`.

### `std::vector<Ship> filter_by_class(...)`

Returns all ships with the requested class in their original order.

### `std::vector<Ship> active_ships(...)`

Returns all active ships in their original order.

## The `Codex` Class

The `Codex` class wraps a private vector:

```cpp
class Codex {
public:
    bool add(Ship ship);
    std::size_t size() const noexcept;
    std::vector<Ship> list() const;
    std::optional<Ship> find_by_name(std::string_view name) const;
    std::vector<Ship> find_by_class(ShipClass ship_class) const;
    bool set_active(std::string_view name, bool active);

private:
    std::vector<Ship> ships_{};
};
```

This is the first place the homework asks you to protect an invariant:

```text
No two ships in a Codex may have the same name.
```

### `bool Codex::add(Ship ship)`

Adds a ship if:

- its name is valid;
- its rating is valid for its class;
- no existing ship has the same name.

Return `true` when the ship is added. Return `false` otherwise.

Hint: call your helper functions. Do not duplicate every validation rule.

### `std::size_t Codex::size() const noexcept`

Returns the number of stored ships.

This method should not modify the codex, so it is marked `const`.

### `std::vector<Ship> Codex::list() const`

Returns a copy of the stored ships in insertion order.

Returning a copy is intentional. It keeps callers from mutating the private
vector directly.

### `std::optional<Ship> Codex::find_by_name(std::string_view name) const`

Finds a ship by exact name. Return `std::nullopt` if no ship matches.

### `std::vector<Ship> Codex::find_by_class(ShipClass ship_class) const`

Returns every matching ship in insertion order.

Hint: reuse `filter_by_class`.

### `bool Codex::set_active(std::string_view name, bool active)`

Finds a ship by name and changes its `active` field.

Return `true` if a ship was found and updated. Return `false` otherwise.

This method mutates the codex, so it is not `const`.

## Suggested Implementation Order

1. Implement `to_string`.
2. Implement `ship_class_from_string`.
3. Implement `is_valid_ship_name`.
4. Implement rating/status helper functions inside `starship_codex.cpp`.
5. Implement `parse_ship_line`.
6. Implement `format_ship`.
7. Implement vector query functions.
8. Implement `Codex`.
9. Add at least three of your own tests.

Do not begin with the `Codex` class. The parser and helpers are easier to test
in isolation.

## Testing

Run:

```bash
cmake --preset bonus-cpp-warmup
cmake --build --preset bonus-cpp-warmup
ctest --preset bonus-cpp-warmup --output-on-failure
```

The provided tests check:

- enum conversion;
- parsing and formatting;
- total and average ratings;
- strongest ship query;
- filtering;
- duplicate-name rejection;
- `Codex` mutation.

These tests are not exhaustive. Add your own tests for:

- empty fields;
- extra separators;
- invalid rating text;
- invalid names;
- duplicate records;
- tie-breaking in `strongest_ship`;
- case sensitivity.

## Debugging Notes

When a test fails, make the failure smaller.

Useful workflow:

```bash
cmake --build --preset bonus-cpp-warmup
./build/bonus-cpp-warmup/bonus_cpp_warmup_tests
```

Then temporarily add a small print inside the failing test or use `gdb`.

Common bugs:

- treating `std::vector<int> v{10}` as ten elements;
- forgetting that `std::string_view::substr` returns another view;
- storing a `std::string_view` inside `Ship`;
- accepting `"Scout"` even though the spec says lowercase only;
- allowing duplicate names in `Codex::add`;
- doing integer division in `average_rating`;
- using `std::isalnum(c)` without casting `c` to `unsigned char`.

## Grading Breakdown

| Area | Weight | Notes |
|:--|:-:|:--|
| Enum/string conversion | 10% | Exact spellings and invalid cases |
| Name and record parsing | 30% | Strict format, validation, `std::optional` |
| Formatting | 10% | Exact output strings |
| Vector queries | 20% | Totals, averages, filters, strongest |
| `Codex` class | 20% | Private state, duplicate prevention, mutation |
| Tests and style | 10% | Additional tests, clean C++ style |

## Restrictions

Allowed:

- C++20 standard library;
- `std::string`, `std::string_view`, `std::vector`, `std::optional`;
- `<algorithm>`, `<charconv>`, `<cctype>`, `<sstream>`, and similar standard
  headers.

Disallowed:

- raw owning pointers;
- `malloc`, `calloc`, `realloc`, `free`;
- third-party libraries;
- changing public function signatures just to make implementation easier;
- printing from library functions.

## Submission Checklist

Before moving on to HW04:

- `cmake --build --preset bonus-cpp-warmup` succeeds;
- `ctest --preset bonus-cpp-warmup --output-on-failure` passes;
- you can explain `std::string` vs `std::string_view`;
- you can explain why `Codex::size` is `const`;
- you can explain why `Codex::list` returns a copy;
- you added at least three tests beyond the provided ones.
