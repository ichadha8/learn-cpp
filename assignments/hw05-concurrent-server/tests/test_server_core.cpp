#include "server_core.h"

#include <cassert>
#include <cstring>
#include <iostream>

static void test_parse_join() {
    command_t command{};
    assert(server_parse_command("JOIN alice", &command) == 0);
    assert(command.type == CMD_JOIN);
    assert(std::strcmp(command.name, "alice") == 0);
}

static void test_join_leave() {
    server_state_t state{};
    assert(server_state_init(&state) == 0);
    int id = server_join(&state, "alice");
    assert(id == 0);
    assert(state.active_count == 1);
    assert(server_leave(&state, id) == 0);
    assert(state.active_count == 0);
    server_state_destroy(&state);
}

static void test_apply_move() {
    server_state_t state{};
    assert(server_state_init(&state) == 0);
    int id = server_join(&state, "bob");
    command_t command{};
    assert(server_parse_command("MOVE 0 LEFT", &command) == 0);
    assert(server_apply_command(&state, &command) == 0);
    assert(state.players[id].last_move == DIR_LEFT);
    server_state_destroy(&state);
}

int main() {
    test_parse_join();
    test_join_leave();
    test_apply_move();
    std::cout << "hw05 tests passed\n";
    return 0;
}
