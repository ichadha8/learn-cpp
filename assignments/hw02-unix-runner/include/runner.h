#ifndef TRAINING_HW02_RUNNER_H
#define TRAINING_HW02_RUNNER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RUNNER_EXITED = 0,
    RUNNER_SIGNALED = 1,
    RUNNER_TIMEOUT = 2,
    RUNNER_ERROR = 3
} runner_state_t;

typedef struct {
    runner_state_t state;
    int code;
    int elapsed_ms;
} runner_result_t;

runner_result_t runner_make_error(void);
runner_result_t runner_classify_wait_status(int status, int elapsed_ms);
runner_result_t runner_run(char *const argv[], int timeout_ms, const char *stdout_path, const char *stderr_path);

#ifdef __cplusplus
}
#endif

#endif
