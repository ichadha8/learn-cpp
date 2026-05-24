#include "fuzzlab.h"

#include <string.h>

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
    return 0; /* TODO: expand into the full mutation suite. */
}
