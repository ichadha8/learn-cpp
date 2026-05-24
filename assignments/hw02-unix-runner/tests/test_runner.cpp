#include "runner.h"

#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

namespace {

std::string read_file(const char* path) {
    std::ifstream file{path, std::ios::binary};
    return std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
}

int wait_status_for_exit(int code) {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        _exit(code);
    }

    int status = 0;
    assert(waitpid(pid, &status, 0) == pid);
    return status;
}

int wait_status_for_signal(int signal_number) {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        raise(signal_number);
        _exit(127);
    }

    int status = 0;
    assert(waitpid(pid, &status, 0) == pid);
    return status;
}

void test_make_error() {
    runner_result_t result = runner_make_error();
    assert(result.state == RUNNER_ERROR);
    assert(result.code == -1);
    assert(result.elapsed_ms == 0);
}

void test_classify_exit_status() {
    runner_result_t result = runner_classify_wait_status(wait_status_for_exit(7), 123);
    assert(result.state == RUNNER_EXITED);
    assert(result.code == 7);
    assert(result.elapsed_ms == 123);
}

void test_classify_signal_status() {
    runner_result_t result = runner_classify_wait_status(wait_status_for_signal(SIGTERM), 55);
    assert(result.state == RUNNER_SIGNALED);
    assert(result.code == SIGTERM);
    assert(result.elapsed_ms == 55);
}

void test_run_true_and_false() {
    char* true_argv[] = {
        const_cast<char*>("/bin/true"),
        nullptr
    };
    runner_result_t result = runner_run(true_argv, 1000, "/dev/null", "/dev/null");
    assert(result.state == RUNNER_EXITED);
    assert(result.code == 0);

    char* false_argv[] = {
        const_cast<char*>("/bin/false"),
        nullptr
    };
    result = runner_run(false_argv, 1000, "/dev/null", "/dev/null");
    assert(result.state == RUNNER_EXITED);
    assert(result.code == 1);
}

void test_stdout_and_stderr_redirection() {
    const char* stdout_path = "hw02_stdout.txt";
    const char* stderr_path = "hw02_stderr.txt";
    std::remove(stdout_path);
    std::remove(stderr_path);

    char* argv[] = {
        const_cast<char*>("/bin/sh"),
        const_cast<char*>("-c"),
        const_cast<char*>("printf out; printf err >&2"),
        nullptr
    };
    runner_result_t result = runner_run(argv, 1000, stdout_path, stderr_path);
    assert(result.state == RUNNER_EXITED);
    assert(result.code == 0);
    assert(read_file(stdout_path) == "out");
    assert(read_file(stderr_path) == "err");

    std::remove(stdout_path);
    std::remove(stderr_path);
}

void test_signaled_child() {
    char* argv[] = {
        const_cast<char*>("/bin/sh"),
        const_cast<char*>("-c"),
        const_cast<char*>("kill -TERM $$"),
        nullptr
    };
    runner_result_t result = runner_run(argv, 1000, "/dev/null", "/dev/null");
    assert(result.state == RUNNER_SIGNALED);
    assert(result.code == SIGTERM);
}

void test_timeout() {
    char* argv[] = {
        const_cast<char*>("/bin/sleep"),
        const_cast<char*>("2"),
        nullptr
    };
    runner_result_t result = runner_run(argv, 100, "/dev/null", "/dev/null");
    assert(result.state == RUNNER_TIMEOUT);
    assert(result.code == -1);
}

void test_invalid_arguments_and_exec_failure() {
    char* missing_argv[] = {
        const_cast<char*>("/definitely/not/a/real/program"),
        nullptr
    };
    runner_result_t result = runner_run(missing_argv, 1000, "/dev/null", "/dev/null");
    assert(result.state == RUNNER_EXITED);
    assert(result.code == 127);

    assert(runner_run(nullptr, 1000, "/dev/null", "/dev/null").state == RUNNER_ERROR);
    assert(runner_run(missing_argv, 0, "/dev/null", "/dev/null").state == RUNNER_ERROR);
    assert(runner_run(missing_argv, 1000, nullptr, "/dev/null").state == RUNNER_ERROR);
    assert(runner_run(missing_argv, 1000, "/dev/null", nullptr).state == RUNNER_ERROR);
}

} // namespace

int main() {
    test_make_error();
    test_classify_exit_status();
    test_classify_signal_status();
    test_run_true_and_false();
    test_stdout_and_stderr_redirection();
    test_signaled_child();
    test_timeout();
    test_invalid_arguments_and_exec_failure();
    std::cout << "hw02 tests passed\n";
    return 0;
}
