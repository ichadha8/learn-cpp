#include "fuzzlab.h"

#include <string.h>

void fuzzlab_corpus_init(fuzzlab_corpus_t *corpus) {
    if (corpus == NULL) {
        return;
    }
    memset(corpus, 0, sizeof *corpus);
}

int fuzzlab_corpus_push(fuzzlab_corpus_t *corpus, const fuzzlab_input_t *input) {
    (void)corpus;
    (void)input;
    return -1; /* TODO */
}

int fuzzlab_load_seeds(const char *seed_dir, fuzzlab_corpus_t *corpus) {
    (void)seed_dir;
    (void)corpus;
    return -1; /* TODO */
}

int fuzzlab_write_input_file(const char *path, const fuzzlab_input_t *input) {
    (void)path;
    (void)input;
    return -1; /* TODO */
}

int fuzzlab_save_artifact(const char *output_dir, fuzzlab_artifact_kind_t kind, int iteration, const fuzzlab_input_t *input, const fuzzlab_result_t *result) {
    (void)output_dir;
    (void)kind;
    (void)iteration;
    (void)input;
    (void)result;
    return -1; /* TODO */
}

int fuzzlab_write_summary(const char *output_dir, const fuzzlab_summary_t *summary) {
    (void)output_dir;
    (void)summary;
    return -1; /* TODO */
}

int fuzzlab_run(const fuzzlab_config_t *config) {
    (void)config;
    return -1; /* TODO */
}
