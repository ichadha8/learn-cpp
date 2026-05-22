#include "pulsekv.hpp"

namespace pulsekv {

Command parse_command(const std::string& line) {
    (void)line;
    return Command{CommandType::Invalid, {}, {}, "TODO"}; /* TODO */
}

std::string execute_command(Store& store, const Command& command) {
    (void)store;
    (void)command;
    return "ERR not implemented\n"; /* TODO */
}

std::string format_stats(const StoreStats& stats) {
    (void)stats;
    return {}; /* TODO */
}

bool is_valid_key(const std::string& key) {
    (void)key;
    return false; /* TODO */
}

} // namespace pulsekv
