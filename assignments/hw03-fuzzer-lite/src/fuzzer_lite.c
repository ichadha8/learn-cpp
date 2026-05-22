#include "fuzzer_lite.h"

#include <stdlib.h>
#include <string.h>

struct fl_node {
    fl_input_t input;
    struct fl_node *next;
};

void fl_queue_init(fl_queue_t *queue) {
    if (queue == NULL) {
        return;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}

void fl_queue_destroy(fl_queue_t *queue) {
    if (queue == NULL) {
        return;
    }

    fl_node_t *node = queue->head;
    while (node != NULL) {
        fl_node_t *next = node->next;
        free(node);
        node = next;
    }
    fl_queue_init(queue);
}

int fl_queue_push(fl_queue_t *queue, const fl_input_t *input) {
    if (queue == NULL || input == NULL) {
        return -1;
    }

    fl_node_t *node = (fl_node_t *)calloc(1, sizeof *node);
    if (node == NULL) {
        return -1;
    }

    node->input = *input;
    if (queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    return 0;
}

int fl_queue_pop(fl_queue_t *queue, fl_input_t *out) {
    if (queue == NULL || out == NULL || queue->head == NULL) {
        return -1;
    }

    fl_node_t *node = queue->head;
    *out = node->input;
    queue->head = node->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    free(node);
    queue->size--;
    return 0;
}

int fl_input_from_cstr(fl_input_t *out, const char *text) {
    if (out == NULL || text == NULL) {
        return -1;
    }

    size_t length = strlen(text);
    if (length > FL_MAX_INPUT) {
        return -1;
    }

    memset(out, 0, sizeof *out);
    memcpy(out->bytes, text, length);
    out->length = length;
    return 0;
}

int fl_mutate_flip_byte(const fl_input_t *input, fl_input_t *out, unsigned int seed) {
    (void)input;
    (void)out;
    (void)seed;
    return -1; /* TODO */
}

int fl_mutate_duplicate(const fl_input_t *input, fl_input_t *out) {
    (void)input;
    (void)out;
    return -1; /* TODO */
}

int fl_mutate_append_int(const fl_input_t *input, fl_input_t *out, int value) {
    (void)input;
    (void)out;
    (void)value;
    return -1; /* TODO */
}
