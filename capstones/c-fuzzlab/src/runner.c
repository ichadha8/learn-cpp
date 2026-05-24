#include "fuzzlab.h"

#include <sys/wait.h>

fuzzlab_result_t fuzzlab_classify_status(int status, int elapsed_ms) {
    fuzzlab_result_t result;
    result.elapsed_ms = elapsed_ms;
    result.aux = -1;
    result.state = FUZZLAB_RUNNER_ERROR;

    if (WIFEXITED(status)) {
        result.state = FUZZLAB_VALID;
        result.aux = WEXITSTATUS(status);
        return result;
    }

    if (WIFSIGNALED(status)) {
        result.state = FUZZLAB_CRASH;
        result.aux = WTERMSIG(status);
        return result;
    }

    return result;
}

fuzzlab_result_t fuzzlab_run_target(char *const argv[], const char *input_path, int timeout_ms) {
    (void)argv;
    (void)input_path;
    (void)timeout_ms;
    fuzzlab_result_t result;
    result.state = FUZZLAB_RUNNER_ERROR;
    result.aux = -1;
    result.elapsed_ms = 0;
    return result; /* TODO */
}
