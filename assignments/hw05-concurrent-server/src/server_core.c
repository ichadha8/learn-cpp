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
    if (state == NULL || name == NULL || name[0] == '\0') {
        return -1;
    }

    if (pthread_mutex_lock(&state->mutex) != 0) {
        return -1;
    }

    int assigned = -1;
    for (int i = 0; i < SERVER_MAX_PLAYERS; i++) {
        if (!state->players[i].active) {
            assigned = i;
            state->players[i].active = 1;
            state->players[i].last_move = DIR_UP;
            snprintf(state->players[i].name, sizeof state->players[i].name, "%s", name);
            state->active_count++;
            break;
        }
    }

    (void)pthread_mutex_unlock(&state->mutex);
    return assigned;
}

int server_leave(server_state_t *state, int player_id) {
    if (state == NULL || player_id < 0 || player_id >= SERVER_MAX_PLAYERS) {
        return -1;
    }

    if (pthread_mutex_lock(&state->mutex) != 0) {
        return -1;
    }

    if (!state->players[player_id].active) {
        (void)pthread_mutex_unlock(&state->mutex);
        return -1;
    }

    memset(&state->players[player_id], 0, sizeof state->players[player_id]);
    state->active_count--;
    (void)pthread_mutex_unlock(&state->mutex);
    return 0;
}

static int parse_direction(const char *text, move_dir_t *out) {
    if (text == NULL || out == NULL) {
        return -1;
    }
    if (strcmp(text, "UP") == 0) {
        *out = DIR_UP;
        return 0;
    }
    if (strcmp(text, "DOWN") == 0) {
        *out = DIR_DOWN;
        return 0;
    }
    if (strcmp(text, "LEFT") == 0) {
        *out = DIR_LEFT;
        return 0;
    }
    if (strcmp(text, "RIGHT") == 0) {
        *out = DIR_RIGHT;
        return 0;
    }
    return -1;
}

int server_parse_command(const char *line, command_t *out) {
    if (line == NULL || out == NULL) {
        return -1;
    }

    memset(out, 0, sizeof *out);
    out->type = CMD_INVALID;

    char op[16] = {0};
    char arg1[SERVER_NAME_MAX + 1] = {0};
    char arg2[SERVER_NAME_MAX + 1] = {0};
    int count = sscanf(line, "%15s %31s %31s", op, arg1, arg2);

    if (count == 1 && strcmp(op, "STATE") == 0) {
        out->type = CMD_STATE;
        return 0;
    }

    if (count == 2 && strcmp(op, "JOIN") == 0) {
        out->type = CMD_JOIN;
        snprintf(out->name, sizeof out->name, "%s", arg1);
        return 0;
    }

    if (count == 2 && strcmp(op, "LEAVE") == 0) {
        int id = -1;
        if (sscanf(arg1, "%d", &id) != 1) {
            return -1;
        }
        out->type = CMD_LEAVE;
        out->player_id = id;
        return 0;
    }

    if (count == 3 && strcmp(op, "MOVE") == 0) {
        int id = -1;
        move_dir_t dir = DIR_UP;
        if (sscanf(arg1, "%d", &id) != 1 || parse_direction(arg2, &dir) != 0) {
            return -1;
        }
        out->type = CMD_MOVE;
        out->player_id = id;
        out->dir = dir;
        return 0;
    }

    return -1;
}

int server_apply_command(server_state_t *state, const command_t *command) {
    if (state == NULL || command == NULL) {
        return -1;
    }

    switch (command->type) {
    case CMD_JOIN:
        return server_join(state, command->name);
    case CMD_LEAVE:
        return server_leave(state, command->player_id);
    case CMD_MOVE:
        if (command->player_id < 0 || command->player_id >= SERVER_MAX_PLAYERS) {
            return -1;
        }
        if (pthread_mutex_lock(&state->mutex) != 0) {
            return -1;
        }
        if (!state->players[command->player_id].active) {
            (void)pthread_mutex_unlock(&state->mutex);
            return -1;
        }
        state->players[command->player_id].last_move = command->dir;
        (void)pthread_mutex_unlock(&state->mutex);
        return 0;
    case CMD_STATE:
        return 0;
    case CMD_INVALID:
        return -1;
    }

    return -1;
}
