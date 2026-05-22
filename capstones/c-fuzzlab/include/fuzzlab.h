#ifndef TRAINING_CAPSTONE_FUZZLAB_H
#define TRAINING_CAPSTONE_FUZZLAB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FUZZLAB_MAX_INPUT 4096
#define FUZZLAB_MAX_CORPUS 128
#define FUZZLAB_MAX_PATH 1024

typedef enum {
    FUZZLAB_VALID = 0,
    FUZZLAB_CRASH = 1,
    FUZZLAB_TIMEOUT = 2,
    FUZZLAB_RUNNER_ERROR = 3
} fuzzlab_state_t;

typedef enum {
    FUZZLAB_ARTIFACT_CRASH = 0,
    FUZZLAB_ARTIFACT_TIMEOUT = 1,
    FUZZLAB_ARTIFACT_INTERESTING = 2
} fuzzlab_artifact_kind_t;

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
    fuzzlab_input_t inputs[FUZZLAB_MAX_CORPUS];
    size_t count;
} fuzzlab_corpus_t;

typedef struct {
    const char *seed_dir;
    const char *output_dir;
    int iterations;
    int timeout_ms;
    char **target_argv;
} fuzzlab_config_t;

typedef struct {
    int total_runs;
    int valid_runs;
    int crashes;
    int timeouts;
    int runner_errors;
    int saved_artifacts;
} fuzzlab_summary_t;

unsigned int fuzzlab_hash_bytes(const unsigned char *bytes, size_t length);
int fuzzlab_input_from_string(fuzzlab_input_t *out, const char *text);
int fuzzlab_mutate(const fuzzlab_input_t *input, fuzzlab_input_t *out, unsigned int seed);
fuzzlab_result_t fuzzlab_classify_status(int status, int elapsed_ms);
fuzzlab_result_t fuzzlab_run_target(char *const argv[], const char *input_path, int timeout_ms);
void fuzzlab_corpus_init(fuzzlab_corpus_t *corpus);
int fuzzlab_corpus_push(fuzzlab_corpus_t *corpus, const fuzzlab_input_t *input);
int fuzzlab_load_seeds(const char *seed_dir, fuzzlab_corpus_t *corpus);
int fuzzlab_write_input_file(const char *path, const fuzzlab_input_t *input);
int fuzzlab_save_artifact(const char *output_dir, fuzzlab_artifact_kind_t kind, int iteration, const fuzzlab_input_t *input, const fuzzlab_result_t *result);
int fuzzlab_write_summary(const char *output_dir, const fuzzlab_summary_t *summary);
int fuzzlab_run(const fuzzlab_config_t *config);

#ifdef __cplusplus
}
#endif

#endif
