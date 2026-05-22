#include "fuzzlab.h"

#include <string.h>
#include <sys/wait.h>

unsigned int fuzzlab_hash_bytes(const unsigned char *bytes, size_t length) {
    unsigned int hash = 2166136261u;
    if (bytes == NULL) {
        return hash;
    }
    for (size_t i = 0; i < length; i++) {
        hash ^= bytes[i];
        hash *= 16777619u;
    }
    return hash;
}

int fuzzlab_input_from_string(fuzzlab_input_t *out, const char *text) {
    if (out == NULL || text == NULL) {
        return -1;
    }
    size_t length = strlen(text);
    if (length > FUZZLAB_MAX_INPUT) {
        return -1;
    }
    memset(out, 0, sizeof *out);
    memcpy(out->bytes, text, length);
    out->length = length;
    return 0;
}

int fuzzlab_mutate(const fuzzlab_input_t *input, fuzzlab_input_t *out, unsigned int seed) {
    if (input == NULL || out == NULL || input->length > FUZZLAB_MAX_INPUT) {
        return -1;
    }
    *out = *input;
    if (out->length == 0) {
        return 0;
    }
    size_t index = seed % out->length;
    out->bytes[index] ^= 0x1u;
    return 0;
}

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

int fuzzlab_run(const fuzzlab_config_t *config) {
    (void)config;
    return -1; /* TODO */
}
