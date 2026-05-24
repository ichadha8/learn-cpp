#ifndef TRAINING_HW05_SERVER_CORE_H
#define TRAINING_HW05_SERVER_CORE_H

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SERVER_MAX_PLAYERS 8
#define SERVER_NAME_MAX 31

typedef enum {
    CMD_INVALID = 0,
    CMD_JOIN,
    CMD_MOVE,
    CMD_LEAVE,
    CMD_STATE
} command_type_t;

typedef enum {
    DIR_UP = 0,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} move_dir_t;

typedef struct {
    command_type_t type;
    int player_id;
    move_dir_t dir;
    char name[SERVER_NAME_MAX + 1];
} command_t;

typedef struct {
    int active;
    char name[SERVER_NAME_MAX + 1];
    move_dir_t last_move;
} player_t;

typedef struct {
    pthread_mutex_t mutex;
    player_t players[SERVER_MAX_PLAYERS];
    int active_count;
} server_state_t;

int server_state_init(server_state_t *state);
void server_state_destroy(server_state_t *state);
int server_join(server_state_t *state, const char *name);
int server_leave(server_state_t *state, int player_id);
int server_parse_command(const char *line, command_t *out);
int server_apply_command(server_state_t *state, const command_t *command);

#ifdef __cplusplus
}
#endif

#endif
