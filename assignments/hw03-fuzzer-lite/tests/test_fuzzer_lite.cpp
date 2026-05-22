#include "fuzzer_lite.h"

#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>

namespace {

void fill_input(fl_input_t* input, unsigned char byte, size_t length) {
    assert(input != nullptr);
    assert(length <= FL_MAX_INPUT);
    std::memset(input, 0, sizeof *input);
    std::memset(input->bytes, byte, length);
    input->length = length;
}

void test_input_from_cstr_validation() {
    fl_input_t input{};
    assert(fl_input_from_cstr(&input, "hello") == 0);
    assert(input.length == static_cast<size_t>(5));
    assert(std::memcmp(input.bytes, "hello", 5) == 0);

    std::string max_input(FL_MAX_INPUT, 'x');
    assert(fl_input_from_cstr(&input, max_input.c_str()) == 0);
    assert(input.length == static_cast<size_t>(FL_MAX_INPUT));

    std::string too_long(FL_MAX_INPUT + 1, 'x');
    assert(fl_input_from_cstr(&input, too_long.c_str()) == -1);
    assert(fl_input_from_cstr(nullptr, "x") == -1);
    assert(fl_input_from_cstr(&input, nullptr) == -1);
}

void test_queue_fifo_and_copy_semantics() {
    fl_queue_t queue{};
    fl_queue_init(&queue);

    fl_input_t first{};
    fl_input_t second{};
    assert(fl_input_from_cstr(&first, "one") == 0);
    assert(fl_input_from_cstr(&second, "two") == 0);
    assert(fl_queue_push(&queue, &first) == 0);
    assert(fl_queue_push(&queue, &second) == 0);
    assert(queue.size == 2);

    first.bytes[0] = 'X';

    fl_input_t out{};
    assert(fl_queue_pop(&queue, &out) == 0);
    assert(out.length == static_cast<size_t>(3));
    assert(std::memcmp(out.bytes, "one", 3) == 0);
    assert(queue.size == 1);
    assert(fl_queue_pop(&queue, &out) == 0);
    assert(std::memcmp(out.bytes, "two", 3) == 0);
    assert(queue.size == 0);
    assert(fl_queue_pop(&queue, &out) == -1);

    fl_queue_destroy(&queue);
}

void test_queue_destroy_and_invalid_arguments() {
    fl_queue_t queue{};
    fl_queue_init(&queue);

    fl_input_t input{};
    assert(fl_input_from_cstr(&input, "seed") == 0);
    assert(fl_queue_push(&queue, &input) == 0);
    assert(fl_queue_push(&queue, &input) == 0);
    fl_queue_destroy(&queue);
    assert(queue.head == nullptr);
    assert(queue.tail == nullptr);
    assert(queue.size == 0);

    fl_input_t out{};
    assert(fl_queue_push(nullptr, &input) == -1);
    assert(fl_queue_push(&queue, nullptr) == -1);
    assert(fl_queue_pop(nullptr, &out) == -1);
    assert(fl_queue_pop(&queue, nullptr) == -1);
    fl_queue_destroy(nullptr);
}

void test_flip_byte_mutation() {
    fl_input_t input{};
    fl_input_t out{};
    assert(fl_input_from_cstr(&input, "aaaa") == 0);
    assert(fl_mutate_flip_byte(&input, &out, 5) == 0);
    assert(out.length == input.length);
    assert(std::memcmp(input.bytes, out.bytes, input.length) != 0);

    int diff_count = 0;
    for (size_t i = 0; i < input.length; i++) {
        if (input.bytes[i] != out.bytes[i]) {
            diff_count++;
            assert(i == 1);
        }
    }
    assert(diff_count == 1);

    fl_input_t empty{};
    assert(fl_mutate_flip_byte(&empty, &out, 0) == 0);
    assert(out.length == static_cast<size_t>(0));
    assert(fl_mutate_flip_byte(nullptr, &out, 0) == -1);
    assert(fl_mutate_flip_byte(&input, nullptr, 0) == -1);
}

void test_duplicate_mutation() {
    fl_input_t input{};
    fl_input_t out{};
    assert(fl_input_from_cstr(&input, "ab") == 0);
    assert(fl_mutate_duplicate(&input, &out) == 0);
    assert(out.length == static_cast<size_t>(4));
    assert(std::memcmp(out.bytes, "abab", 4) == 0);

    fl_input_t empty{};
    assert(fl_mutate_duplicate(&empty, &out) == 0);
    assert(out.length == static_cast<size_t>(0));

    fill_input(&input, 'z', FL_MAX_INPUT - 1);
    assert(fl_mutate_duplicate(&input, &out) == 0);
    assert(out.length == static_cast<size_t>(FL_MAX_INPUT));
    for (size_t i = 0; i < static_cast<size_t>(FL_MAX_INPUT); i++) {
        assert(out.bytes[i] == 'z');
    }

    assert(fl_mutate_duplicate(nullptr, &out) == -1);
    assert(fl_mutate_duplicate(&input, nullptr) == -1);
}

void test_append_int_mutation() {
    fl_input_t input{};
    fl_input_t out{};
    assert(fl_input_from_cstr(&input, "x") == 0);
    assert(fl_mutate_append_int(&input, &out, 123) == 0);
    assert(out.length == static_cast<size_t>(4));
    assert(std::memcmp(out.bytes, "x123", 4) == 0);

    assert(fl_mutate_append_int(&input, &out, -1) == 0);
    assert(out.length == static_cast<size_t>(3));
    assert(std::memcmp(out.bytes, "x-1", 3) == 0);

    fill_input(&input, 'a', FL_MAX_INPUT - 1);
    assert(fl_mutate_append_int(&input, &out, 999) == 0);
    assert(out.length == static_cast<size_t>(FL_MAX_INPUT));
    assert(out.bytes[FL_MAX_INPUT - 1] == '9');

    assert(fl_mutate_append_int(nullptr, &out, 1) == -1);
    assert(fl_mutate_append_int(&input, nullptr, 1) == -1);
}

} // namespace

int main() {
    test_input_from_cstr_validation();
    test_queue_fifo_and_copy_semantics();
    test_queue_destroy_and_invalid_arguments();
    test_flip_byte_mutation();
    test_duplicate_mutation();
    test_append_int_mutation();
    std::cout << "hw03 tests passed\n";
    return 0;
}
