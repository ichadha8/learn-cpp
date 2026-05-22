#include "raii.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

using training::OwnedBuffer;
using training::UniqueFd;

static void test_unique_fd_is_move_only() {
    static_assert(!std::is_copy_constructible_v<UniqueFd>);
    static_assert(!std::is_copy_assignable_v<UniqueFd>);
    static_assert(std::is_move_constructible_v<UniqueFd>);
    static_assert(std::is_move_assignable_v<UniqueFd>);
}

static void test_release() {
    UniqueFd fd{12345};
    assert(fd.valid());
    int raw = fd.release();
    assert(raw == 12345);
    assert(!fd.valid());
}

static void test_buffer_append() {
    OwnedBuffer buffer;
    const unsigned char bytes[] = {'h', 'i'};
    buffer.append(bytes, 2);
    assert(buffer.size() == 2);
    assert(buffer.as_string() == "hi");
}

static void test_write_and_read_file() {
    std::string path = "hw04_tmp.txt";
    assert(training::write_file(path, "abc"));
    auto contents = training::read_file(path);
    assert(contents.has_value());
    assert(*contents == "abc");
}

int main() {
    test_unique_fd_is_move_only();
    test_release();
    test_buffer_append();
    test_write_and_read_file();
    std::cout << "hw04 tests passed\n";
    return 0;
}
