#include "fuzzlab.h"

#include <cassert>
#include <cstring>
#include <iostream>

static void test_hash_is_stable() {
    const unsigned char bytes[] = {'a', 'b', 'c'};
    assert(fuzzlab_hash_bytes(bytes, 3) == fuzzlab_hash_bytes(bytes, 3));
}

static void test_input_from_string() {
    fuzzlab_input_t input{};
    assert(fuzzlab_input_from_string(&input, "hello") == 0);
    assert(input.length == 5);
    assert(std::memcmp(input.bytes, "hello", 5) == 0);
}

static void test_mutate_changes_nonempty_input() {
    fuzzlab_input_t input{};
    fuzzlab_input_t out{};
    assert(fuzzlab_input_from_string(&input, "aaaa") == 0);
    assert(fuzzlab_mutate(&input, &out, 0) == 0);
    assert(out.length == input.length);
    assert(std::memcmp(input.bytes, out.bytes, input.length) != 0);
}

int main() {
    test_hash_is_stable();
    test_input_from_string();
    test_mutate_changes_nonempty_input();
    std::cout << "fuzzlab tests passed\n";
    return 0;
}
