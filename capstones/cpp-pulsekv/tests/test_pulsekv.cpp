#include "pulsekv.hpp"

#include <cassert>
#include <iostream>

static void test_store_set_get_delete() {
    pulsekv::Store store;
    store.set("name", "systems");
    auto value = store.get("name");
    assert(value.has_value());
    assert(*value == "systems");
    assert(store.erase("name"));
    assert(!store.get("name").has_value());
}

static void test_parse_set_preserves_value_spaces() {
    auto command = pulsekv::parse_command("SET greeting hello world");
    assert(command.type == pulsekv::CommandType::Set);
    assert(command.key == "greeting");
    assert(command.value == "hello world");
}

static void test_execute_get_missing() {
    pulsekv::Store store;
    auto command = pulsekv::parse_command("GET nope");
    assert(pulsekv::execute_command(store, command) == "NOT_FOUND\n");
}

int main() {
    test_store_set_get_delete();
    test_parse_set_preserves_value_spaces();
    test_execute_get_missing();
    std::cout << "pulsekv tests passed\n";
    return 0;
}
