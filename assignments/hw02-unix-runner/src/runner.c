#include "runner.h"

#include <sys/types.h>
#include <sys/wait.h>

runner_result_t runner_make_error(void) {
    runner_result_t result;
    result.state = RUNNER_ERROR;
    result.code = -1;
    result.elapsed_ms = 0;
    return result;
}

runner_result_t runner_classify_wait_status(int status, int elapsed_ms) {
    runner_result_t result;
    result.elapsed_ms = elapsed_ms;

    if (WIFEXITED(status)) {
        result.state = RUNNER_EXITED;
        result.code = WEXITSTATUS(status);
        return result;
    }

    if (WIFSIGNALED(status)) {
        result.state = RUNNER_SIGNALED;
        result.code = WTERMSIG(status);
        return result;
    }

    return runner_make_error();
}

runner_result_t runner_run(char *const argv[], int timeout_ms, const char *stdout_path, const char *stderr_path) {
    (void)argv;
    (void)timeout_ms;
    (void)stdout_path;
    (void)stderr_path;
    return runner_make_error(); /* TODO */
}
