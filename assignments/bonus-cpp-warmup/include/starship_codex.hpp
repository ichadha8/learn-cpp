#ifndef TRAINING_BONUS_CPP_WARMUP_STARSHIP_CODEX_HPP
#define TRAINING_BONUS_CPP_WARMUP_STARSHIP_CODEX_HPP

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace training {

enum class ShipClass {
    Scout,
    Freighter,
    Cruiser,
    Carrier,
    Unknown
};

struct Ship {
    std::string name{};
    ShipClass ship_class{ShipClass::Unknown};
    int rating{0};
    bool active{false};
};

std::string to_string(ShipClass ship_class);
std::optional<ShipClass> ship_class_from_string(std::string_view text);
bool is_valid_ship_name(std::string_view name);
std::optional<Ship> parse_ship_line(std::string_view line);
std::string format_ship(const Ship& ship);

int total_rating(const std::vector<Ship>& ships);
double average_rating(const std::vector<Ship>& ships);
std::optional<Ship> strongest_ship(const std::vector<Ship>& ships);
std::vector<Ship> filter_by_class(const std::vector<Ship>& ships, ShipClass ship_class);
std::vector<Ship> active_ships(const std::vector<Ship>& ships);

class Codex {
public:
    bool add(Ship ship);
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] std::vector<Ship> list() const;
    [[nodiscard]] std::optional<Ship> find_by_name(std::string_view name) const;
    [[nodiscard]] std::vector<Ship> find_by_class(ShipClass ship_class) const;
    bool set_active(std::string_view name, bool active);

private:
    std::vector<Ship> ships_{};
};

} // namespace training

#endif
