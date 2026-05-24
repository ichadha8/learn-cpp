#include "server_core.h"

#include <stdio.h>
#include <string.h>

int server_state_init(server_state_t *state) {
    if (state == NULL) {
        return -1;
    }
    memset(state, 0, sizeof *state);
    if (pthread_mutex_init(&state->mutex, NULL) != 0) {
        return -1;
    }
    return 0;
}

void server_state_destroy(server_state_t *state) {
    if (state == NULL) {
        return;
    }
    (void)pthread_mutex_destroy(&state->mutex);
}

int server_join(server_state_t *state, const char *name) {
    (void)state;
    (void)name;
    return -1; /* TODO */
}

int server_leave(server_state_t *state, int player_id) {
    (void)state;
    (void)player_id;
    return -1; /* TODO */
}

int server_parse_command(const char *line, command_t *out) {
    (void)line;
    (void)out;
    return -1; /* TODO */
}

int server_apply_command(server_state_t *state, const command_t *command) {
    (void)state;
    (void)command;
    return -1; /* TODO */
}
