#include "pulsekv.hpp"

#include <sstream>

namespace pulsekv {

void Store::set(std::string key, std::string value) {
    (void)key;
    (void)value;
    /* TODO */
}

std::optional<std::string> Store::get(const std::string& key) {
    (void)key;
    return std::nullopt; /* TODO */
}

bool Store::erase(const std::string& key) {
    (void)key;
    return false; /* TODO */
}

StoreStats Store::stats() const {
    return {}; /* TODO */
}

Command parse_command(const std::string& line) {
    (void)line;
    return Command{CommandType::Invalid, {}, {}, "TODO"}; /* TODO */
}

std::string execute_command(Store& store, const Command& command) {
    (void)store;
    (void)command;
    return "ERR not implemented\n"; /* TODO */
}

} // namespace pulsekv
