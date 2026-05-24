#include "starship_codex.hpp"

namespace training {

std::string to_string(ShipClass ship_class) {
    (void)ship_class;
    return "unknown"; /* TODO */
}

std::optional<ShipClass> ship_class_from_string(std::string_view text) {
    (void)text;
    return std::nullopt; /* TODO */
}

bool is_valid_ship_name(std::string_view name) {
    (void)name;
    return false; /* TODO */
}

std::optional<Ship> parse_ship_line(std::string_view line) {
    (void)line;
    return std::nullopt; /* TODO */
}

std::string format_ship(const Ship& ship) {
    (void)ship;
    return {}; /* TODO */
}

int total_rating(const std::vector<Ship>& ships) {
    (void)ships;
    return 0; /* TODO */
}

double average_rating(const std::vector<Ship>& ships) {
    (void)ships;
    return 0.0; /* TODO */
}

std::optional<Ship> strongest_ship(const std::vector<Ship>& ships) {
    (void)ships;
    return std::nullopt; /* TODO */
}

std::vector<Ship> filter_by_class(const std::vector<Ship>& ships, ShipClass ship_class) {
    (void)ships;
    (void)ship_class;
    return {}; /* TODO */
}

std::vector<Ship> active_ships(const std::vector<Ship>& ships) {
    (void)ships;
    return {}; /* TODO */
}

bool Codex::add(Ship ship) {
    (void)ship;
    return false; /* TODO */
}

std::size_t Codex::size() const noexcept {
    return 0; /* TODO */
}

std::vector<Ship> Codex::list() const {
    return {}; /* TODO */
}

std::optional<Ship> Codex::find_by_name(std::string_view name) const {
    (void)name;
    return std::nullopt; /* TODO */
}

std::vector<Ship> Codex::find_by_class(ShipClass ship_class) const {
    (void)ship_class;
    return {}; /* TODO */
}

bool Codex::set_active(std::string_view name, bool active) {
    (void)name;
    (void)active;
    return false; /* TODO */
}

} // namespace training
