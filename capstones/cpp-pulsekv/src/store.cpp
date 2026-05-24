#include "pulsekv.hpp"

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

} // namespace pulsekv
