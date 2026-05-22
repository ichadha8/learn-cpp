#include "runner.h"

#include <cassert>
#include <iostream>
#include <sys/wait.h>

static void test_classify_exit_status() {
    int status = 0;
    status = 7 << 8;
    runner_result_t result = runner_classify_wait_status(status, 123);
    assert(result.state == RUNNER_EXITED);
    assert(result.code == 7);
    assert(result.elapsed_ms == 123);
}

static void test_run_true() {
    char *argv[] = {
        const_cast<char *>("/bin/true"),
        nullptr
    };
    runner_result_t result = runner_run(argv, 1000, "/dev/null", "/dev/null");
    assert(result.state == RUNNER_EXITED);
    assert(result.code == 0);
}

static void test_timeout() {
    char *argv[] = {
        const_cast<char *>("/bin/sleep"),
        const_cast<char *>("2"),
        nullptr
    };
    runner_result_t result = runner_run(argv, 100, "/dev/null", "/dev/null");
    assert(result.state == RUNNER_TIMEOUT);
}

int main() {
    test_classify_exit_status();
    test_run_true();
    test_timeout();
    std::cout << "hw02 tests passed\n";
    return 0;
}
