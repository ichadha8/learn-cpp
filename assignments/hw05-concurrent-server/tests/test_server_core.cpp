#include "server_core.h"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {

void test_state_init_and_destroy() {
    server_state_t state{};
    assert(server_state_init(&state) == 0);
    assert(state.active_count == 0);
    for (int i = 0; i < SERVER_MAX_PLAYERS; i++) {
        assert(state.players[i].active == 0);
        assert(state.players[i].name[0] == '\0');
    }
    server_state_destroy(&state);
    server_state_destroy(nullptr);
    assert(server_state_init(nullptr) == -1);
}

void test_join_assigns_first_free_slot_and_truncates() {
    server_state_t state{};
    assert(server_state_init(&state) == 0);

    int alice = server_join(&state, "alice");
    int bob = server_join(&state, "bob");
    assert(alice == 0);
    assert(bob == 1);
    assert(state.active_count == 2);
    assert(state.players[alice].active == 1);
    assert(std::strcmp(state.players[alice].name, "alice") == 0);
    assert(state.players[alice].last_move == DIR_UP);

    assert(server_leave(&state, alice) == 0);
    int carol = server_join(&state, "carol");
    assert(carol == alice);

    std::string long_name(SERVER_NAME_MAX + 20, 'x');
    int id = server_join(&state, long_name.c_str());
    assert(id >= 0);
    assert(std::strlen(state.players[id].name) == static_cast<size_t>(SERVER_NAME_MAX));
    assert(state.players[id].name[SERVER_NAME_MAX] == '\0');

    assert(server_join(nullptr, "bad") == -1);
    assert(server_join(&state, nullptr) == -1);
    assert(server_join(&state, "") == -1);
    server_state_destroy(&state);
}

void test_join_full_and_leave_validation() {
    server_state_t state{};
    assert(server_state_init(&state) == 0);

    for (int i = 0; i < SERVER_MAX_PLAYERS; i++) {
        std::string name = "p" + std::to_string(i);
        assert(server_join(&state, name.c_str()) == i);
    }
    assert(state.active_count == SERVER_MAX_PLAYERS);
    assert(server_join(&state, "overflow") == -1);

    assert(server_leave(&state, -1) == -1);
    assert(server_leave(&state, SERVER_MAX_PLAYERS) == -1);
    assert(server_leave(&state, 3) == 0);
    assert(state.active_count == SERVER_MAX_PLAYERS - 1);
    assert(state.players[3].active == 0);
    assert(state.players[3].name[0] == '\0');
    assert(server_leave(&state, 3) == -1);
    assert(server_join(&state, "replacement") == 3);

    server_state_destroy(&state);
}

void test_parse_valid_commands() {
    command_t command{};
    assert(server_parse_command("JOIN alice", &command) == 0);
    assert(command.type == CMD_JOIN);
    assert(std::strcmp(command.name, "alice") == 0);

    assert(server_parse_command("MOVE 7 UP", &command) == 0);
    assert(command.type == CMD_MOVE);
    assert(command.player_id == 7);
    assert(command.dir == DIR_UP);

    assert(server_parse_command("MOVE 2 DOWN", &command) == 0);
    assert(command.dir == DIR_DOWN);
    assert(server_parse_command("MOVE 2 LEFT", &command) == 0);
    assert(command.dir == DIR_LEFT);
    assert(server_parse_command("MOVE 2 RIGHT", &command) == 0);
    assert(command.dir == DIR_RIGHT);

    assert(server_parse_command("LEAVE 4", &command) == 0);
    assert(command.type == CMD_LEAVE);
    assert(command.player_id == 4);

    assert(server_parse_command("STATE", &command) == 0);
    assert(command.type == CMD_STATE);
}

void test_parse_invalid_commands() {
    command_t command{};
    assert(server_parse_command(nullptr, &command) == -1);
    assert(server_parse_command("STATE", nullptr) == -1);
    assert(server_parse_command("", &command) == -1);
    assert(server_parse_command("HELLO alice", &command) == -1);
    assert(server_parse_command("JOIN", &command) == -1);
    assert(server_parse_command("JOIN alice extra", &command) == -1);
    assert(server_parse_command("MOVE 1 NORTH", &command) == -1);
    assert(server_parse_command("MOVE x UP", &command) == -1);
    assert(server_parse_command("MOVE -1 UP", &command) == -1);
    assert(server_parse_command("LEAVE", &command) == -1);
    assert(server_parse_command("LEAVE nope", &command) == -1);
    assert(server_parse_command("STATE extra", &command) == -1);
    assert(command.type == CMD_INVALID);
}

void test_apply_commands() {
    server_state_t state{};
    assert(server_state_init(&state) == 0);

    command_t command{};
    assert(server_parse_command("JOIN alice", &command) == 0);
    int id = server_apply_command(&state, &command);
    assert(id == 0);
    assert(state.active_count == 1);

    assert(server_parse_command("MOVE 0 LEFT", &command) == 0);
    assert(server_apply_command(&state, &command) == 0);
    assert(state.players[0].last_move == DIR_LEFT);

    assert(server_parse_command("STATE", &command) == 0);
    assert(server_apply_command(&state, &command) == 0);

    assert(server_parse_command("LEAVE 0", &command) == 0);
    assert(server_apply_command(&state, &command) == 0);
    assert(state.active_count == 0);

    assert(server_parse_command("MOVE 0 RIGHT", &command) == 0);
    assert(server_apply_command(&state, &command) == -1);
    assert(server_apply_command(nullptr, &command) == -1);
    assert(server_apply_command(&state, nullptr) == -1);

    command.type = CMD_INVALID;
    assert(server_apply_command(&state, &command) == -1);
    server_state_destroy(&state);
}

void test_concurrent_joins_fill_unique_slots() {
    server_state_t state{};
    assert(server_state_init(&state) == 0);

    std::array<int, SERVER_MAX_PLAYERS> ids{};
    std::array<std::string, SERVER_MAX_PLAYERS> names{};
    std::vector<std::thread> threads;
    for (int i = 0; i < SERVER_MAX_PLAYERS; i++) {
        ids[static_cast<size_t>(i)] = -1;
        names[static_cast<size_t>(i)] = "thread_" + std::to_string(i);
        threads.emplace_back([&state, &ids, &names, i]() {
            ids[static_cast<size_t>(i)] = server_join(&state, names[static_cast<size_t>(i)].c_str());
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    std::array<bool, SERVER_MAX_PLAYERS> seen{};
    for (int id : ids) {
        assert(id >= 0);
        assert(id < SERVER_MAX_PLAYERS);
        assert(!seen[static_cast<size_t>(id)]);
        seen[static_cast<size_t>(id)] = true;
    }
    assert(state.active_count == SERVER_MAX_PLAYERS);
    server_state_destroy(&state);
}

} // namespace

int main() {
    test_state_init_and_destroy();
    test_join_assigns_first_free_slot_and_truncates();
    test_join_full_and_leave_validation();
    test_parse_valid_commands();
    test_parse_invalid_commands();
    test_apply_commands();
    test_concurrent_joins_fill_unique_slots();
    std::cout << "hw05 tests passed\n";
    return 0;
}
