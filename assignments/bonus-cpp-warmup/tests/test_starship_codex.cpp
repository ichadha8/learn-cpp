#include "starship_codex.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

namespace {

bool close_enough(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 0.0001;
}

void test_ship_class_conversion() {
    const auto scout{training::ship_class_from_string("scout")};
    assert(scout.has_value());
    assert(*scout == training::ShipClass::Scout);
    assert(training::to_string(training::ShipClass::Carrier) == "carrier");
    assert(!training::ship_class_from_string("battleship").has_value());
}

void test_parse_and_format() {
    const auto parsed{training::parse_ship_line("Aurora|scout|8|active")};
    assert(parsed.has_value());
    assert(parsed->name == "Aurora");
    assert(parsed->ship_class == training::ShipClass::Scout);
    assert(parsed->rating == 8);
    assert(parsed->active);
    assert(training::format_ship(*parsed) == "Aurora [scout] rating=8 active");
}

void test_collection_queries() {
    const std::vector<training::Ship> ships{
        {"Aurora", training::ShipClass::Scout, 8, true},
        {"Borealis", training::ShipClass::Freighter, 4, false},
        {"Calypso", training::ShipClass::Cruiser, 12, true},
        {"Drift", training::ShipClass::Scout, 6, false},
    };

    assert(training::total_rating(ships) == 30);
    assert(close_enough(training::average_rating(ships), 7.5));

    const auto strongest{training::strongest_ship(ships)};
    assert(strongest.has_value());
    assert(strongest->name == "Calypso");

    const auto scouts{training::filter_by_class(ships, training::ShipClass::Scout)};
    assert(scouts.size() == 2);

    const auto active{training::active_ships(ships)};
    assert(active.size() == 2);
}

void test_codex_class() {
    training::Codex codex{};
    assert(codex.add({"Aurora", training::ShipClass::Scout, 8, true}));
    assert(!codex.add({"Aurora", training::ShipClass::Cruiser, 12, false}));
    assert(codex.add({"Calypso", training::ShipClass::Cruiser, 12, true}));
    assert(codex.size() == 2);

    const auto found{codex.find_by_name("Aurora")};
    assert(found.has_value());
    assert(found->rating == 8);

    assert(codex.set_active("Aurora", false));
    const auto updated{codex.find_by_name("Aurora")};
    assert(updated.has_value());
    assert(!updated->active);

    const auto cruisers{codex.find_by_class(training::ShipClass::Cruiser)};
    assert(cruisers.size() == 1);
    assert(cruisers.front().name == "Calypso");
}

} // namespace

int main() {
    test_ship_class_conversion();
    test_parse_and_format();
    test_collection_queries();
    test_codex_class();
    std::cout << "bonus_cpp_warmup_tests passed\n";
    return 0;
}
