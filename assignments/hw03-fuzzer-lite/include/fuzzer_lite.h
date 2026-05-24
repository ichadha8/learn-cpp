#ifndef TRAINING_HW03_FUZZER_LITE_H
#define TRAINING_HW03_FUZZER_LITE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FL_MAX_INPUT 1024

typedef struct {
    unsigned char bytes[FL_MAX_INPUT];
    size_t length;
} fl_input_t;

typedef struct fl_node fl_node_t;

typedef struct {
    fl_node_t *head;
    fl_node_t *tail;
    size_t size;
} fl_queue_t;

void fl_queue_init(fl_queue_t *queue);
void fl_queue_destroy(fl_queue_t *queue);
int fl_queue_push(fl_queue_t *queue, const fl_input_t *input);
int fl_queue_pop(fl_queue_t *queue, fl_input_t *out);

int fl_input_from_cstr(fl_input_t *out, const char *text);
int fl_mutate_flip_byte(const fl_input_t *input, fl_input_t *out, unsigned int seed);
int fl_mutate_duplicate(const fl_input_t *input, fl_input_t *out);
int fl_mutate_append_int(const fl_input_t *input, fl_input_t *out, int value);

#ifdef __cplusplus
}
#endif

#endif
