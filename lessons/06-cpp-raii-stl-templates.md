# Lesson 06: C++ RAII, STL, and Templates

## Standard Library First

LearnCpp notes that the C++ standard library provides containers, algorithms,
and iterators so you do not have to write and debug common data structures from
scratch. That is the right instinct for production C++.

Prefer:

- `std::vector` over manual dynamic arrays;
- `std::string` over C strings;
- `std::unordered_map` over hand-rolled hash tables;
- algorithms over open-coded loops when they make intent clearer.

## Containers

Common choices:

- `std::vector<T>`: contiguous dynamic array, default choice.
- `std::array<T, N>`: fixed-size array with value semantics.
- `std::deque<T>`: efficient push/pop at both ends.
- `std::unordered_map<K, V>`: hash table.
- `std::map<K, V>`: ordered tree.

The most important performance question is often: "Is the data contiguous?"

## Iterators

Iterators generalize positions in containers. The `end()` iterator points one
past the last element, which makes loops simple:

```cpp
for (auto it = values.begin(); it != values.end(); ++it) {
    use(*it);
}
```

Prefer range-for when possible:

```cpp
for (const auto& value : values) {
    use(value);
}
```

## Algorithms

Use algorithms to express intent:

```cpp
auto found = std::find(values.begin(), values.end(), target);
std::sort(values.begin(), values.end());
auto n = std::count_if(values.begin(), values.end(), predicate);
```

Algorithms are pre-tested, generic, and often optimized well.

## Templates

Templates let you write code over types:

```cpp
template <typename T>
T clamp_to_zero(T value) {
    return value < T{} ? T{} : value;
}
```

Use templates when the algorithm is genuinely type-independent. Do not template
everything just because you can.

## RAII Wrappers Around C APIs

Many C APIs return handles:

- file descriptors;
- sockets;
- `FILE*`;
- mutexes;
- allocated memory.

C++ wrappers should:

- acquire in constructor or factory;
- release in destructor;
- delete copy;
- implement move;
- expose minimal raw access.

Example:

```cpp
class UniqueFd {
public:
    UniqueFd() noexcept = default;
    explicit UniqueFd(int fd) noexcept;
    ~UniqueFd();

    int get() const noexcept;
    int release() noexcept;
    void reset(int fd = -1) noexcept;
};
```

## Smart Pointers

Use `std::unique_ptr<T>` for unique ownership. It should be your default smart
pointer.

Use `std::shared_ptr<T>` only when ownership is truly shared. Shared ownership
has runtime overhead and can hide design confusion.

Use raw pointers or references for non-owning access.

## Practice

Implement:

- `UniqueFd`
- `OwnedBuffer`
- `CommandRunner`

Then write tests for:

- destructor cleanup;
- move construction;
- move assignment;
- no copying;
- command exit status.

## Interview Angle

You should be able to answer:

- Why is `std::vector` often faster than a linked list?
- What is an iterator?
- When is `std::shared_ptr` the wrong choice?
- How do you wrap a C resource safely in C++?
