#include "raii.hpp"

#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <type_traits>
#include <unistd.h>
#include <utility>
#include <vector>

using training::OwnedBuffer;
using training::UniqueFd;

namespace {

bool fd_is_closed(int fd) {
    errno = 0;
    return fcntl(fd, F_GETFD) == -1 && errno == EBADF;
}

std::string temp_path(const char* name) {
    return std::string{name} + "_" + std::to_string(static_cast<long long>(getpid())) + ".txt";
}

void test_unique_fd_type_traits() {
    static_assert(!std::is_copy_constructible_v<UniqueFd>);
    static_assert(!std::is_copy_assignable_v<UniqueFd>);
    static_assert(std::is_move_constructible_v<UniqueFd>);
    static_assert(std::is_move_assignable_v<UniqueFd>);
}

void test_release_transfers_ownership_to_caller() {
    int fds[2]{};
    assert(pipe(fds) == 0);
    UniqueFd read_end{fds[0]};
    UniqueFd write_end{fds[1]};

    int raw = read_end.release();
    assert(raw == fds[0]);
    assert(!read_end.valid());
    assert(!fd_is_closed(raw));
    assert(close(raw) == 0);
}

void test_reset_closes_previous_descriptor() {
    int fds[2]{};
    assert(pipe(fds) == 0);
    int old_read = fds[0];
    UniqueFd read_end{fds[0]};
    UniqueFd write_end{fds[1]};

    read_end.reset();
    assert(!read_end.valid());
    assert(fd_is_closed(old_read));
}

void test_move_constructor_and_assignment() {
    int first[2]{};
    int second[2]{};
    assert(pipe(first) == 0);
    assert(pipe(second) == 0);

    int first_read = first[0];
    UniqueFd source{first[0]};
    UniqueFd first_write{first[1]};
    UniqueFd moved{std::move(source)};
    assert(!source.valid());
    assert(moved.get() == first_read);

    int previous = moved.get();
    UniqueFd other{second[0]};
    UniqueFd second_write{second[1]};
    moved = std::move(other);
    assert(!other.valid());
    assert(moved.get() == second[0]);
    assert(fd_is_closed(previous));
}

void test_owned_buffer_basics() {
    OwnedBuffer buffer;
    assert(buffer.size() == static_cast<std::size_t>(0));

    const unsigned char hi[] = {'h', 'i'};
    buffer.append(hi, 2);
    assert(buffer.size() == static_cast<std::size_t>(2));
    assert(buffer.as_string() == "hi");

    const unsigned char bang[] = {'!'};
    buffer.append(bang, 1);
    assert(buffer.size() == static_cast<std::size_t>(3));
    assert(buffer.as_string() == "hi!");

    OwnedBuffer sized{4};
    assert(sized.size() == static_cast<std::size_t>(4));
    unsigned char* data = sized.data();
    assert(data != nullptr);
    data[0] = 'A';
    assert(sized.as_string().size() == static_cast<std::size_t>(4));
    assert(sized.as_string()[0] == 'A');
}

void test_file_read_write_roundtrip_and_failure() {
    const std::string path = temp_path("hw04_tmp");
    std::remove(path.c_str());

    assert(training::write_file(path, "abc"));
    auto contents = training::read_file(path);
    assert(contents.has_value());
    assert(*contents == "abc");

    assert(training::write_file(path, "replacement"));
    contents = training::read_file(path);
    assert(contents.has_value());
    assert(*contents == "replacement");

    std::remove(path.c_str());
    assert(!training::read_file(path).has_value());
}

void test_command_runner_status() {
    training::CommandResult result = training::run_command_capture_status({"/bin/true"});
    assert(!result.signaled);
    assert(result.exit_code == 0);

    result = training::run_command_capture_status({"/bin/false"});
    assert(!result.signaled);
    assert(result.exit_code == 1);

    result = training::run_command_capture_status({"/bin/sh", "-c", "kill -TERM $$"});
    assert(result.signaled);
    assert(result.signal_number == SIGTERM);

    result = training::run_command_capture_status({});
    assert(result.exit_code == -1);
}

} // namespace

int main() {
    test_unique_fd_type_traits();
    test_release_transfers_ownership_to_caller();
    test_reset_closes_previous_descriptor();
    test_move_constructor_and_assignment();
    test_owned_buffer_basics();
    test_file_read_write_roundtrip_and_failure();
    test_command_runner_status();
    std::cout << "hw04 tests passed\n";
    return 0;
}
