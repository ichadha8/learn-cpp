#ifndef TRAINING_CAPSTONE_FUZZLAB_H
#define TRAINING_CAPSTONE_FUZZLAB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FUZZLAB_MAX_INPUT 4096

typedef enum {
    FUZZLAB_VALID = 0,
    FUZZLAB_CRASH = 1,
    FUZZLAB_TIMEOUT = 2,
    FUZZLAB_RUNNER_ERROR = 3
} fuzzlab_state_t;

typedef struct {
    fuzzlab_state_t state;
    int aux;
    int elapsed_ms;
} fuzzlab_result_t;

typedef struct {
    unsigned char bytes[FUZZLAB_MAX_INPUT];
    size_t length;
} fuzzlab_input_t;

typedef struct {
    const char *seed_dir;
    const char *output_dir;
    int iterations;
    int timeout_ms;
    char **target_argv;
} fuzzlab_config_t;

unsigned int fuzzlab_hash_bytes(const unsigned char *bytes, size_t length);
int fuzzlab_input_from_string(fuzzlab_input_t *out, const char *text);
int fuzzlab_mutate(const fuzzlab_input_t *input, fuzzlab_input_t *out, unsigned int seed);
fuzzlab_result_t fuzzlab_classify_status(int status, int elapsed_ms);
fuzzlab_result_t fuzzlab_run_target(char *const argv[], const char *input_path, int timeout_ms);
int fuzzlab_run(const fuzzlab_config_t *config);

#ifdef __cplusplus
}
#endif

#endif
