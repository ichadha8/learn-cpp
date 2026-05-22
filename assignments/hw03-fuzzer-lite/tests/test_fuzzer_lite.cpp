#include "fuzzer_lite.h"

#include <cassert>
#include <cstring>
#include <iostream>

static void test_queue_roundtrip() {
    fl_queue_t queue{};
    fl_queue_init(&queue);

    fl_input_t input{};
    assert(fl_input_from_cstr(&input, "seed") == 0);
    assert(fl_queue_push(&queue, &input) == 0);
    assert(queue.size == 1);

    fl_input_t out{};
    assert(fl_queue_pop(&queue, &out) == 0);
    assert(out.length == 4);
    assert(std::memcmp(out.bytes, "seed", 4) == 0);
    assert(queue.size == 0);

    fl_queue_destroy(&queue);
}

static void test_duplicate_mutation() {
    fl_input_t input{};
    fl_input_t out{};
    assert(fl_input_from_cstr(&input, "ab") == 0);
    assert(fl_mutate_duplicate(&input, &out) == 0);
    assert(out.length == 4);
    assert(std::memcmp(out.bytes, "abab", 4) == 0);
}

static void test_flip_byte_changes_one_byte() {
    fl_input_t input{};
    fl_input_t out{};
    assert(fl_input_from_cstr(&input, "aaaa") == 0);
    assert(fl_mutate_flip_byte(&input, &out, 1) == 0);
    assert(out.length == input.length);
    assert(std::memcmp(input.bytes, out.bytes, input.length) != 0);
}

int main() {
    test_queue_roundtrip();
    test_duplicate_mutation();
    test_flip_byte_changes_one_byte();
    std::cout << "hw03 tests passed\n";
    return 0;
}
