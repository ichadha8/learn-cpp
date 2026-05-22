#include "pulsekv.hpp"

#include <sstream>

namespace pulsekv {

void Store::set(std::string key, std::string value) {
    std::lock_guard<std::mutex> lock(mutex_);
    values_[std::move(key)] = std::move(value);
    stats_.set_count++;
    stats_.keys = values_.size();
}

std::optional<std::string> Store::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    stats_.get_count++;
    auto it = values_.find(key);
    if (it == values_.end()) {
        stats_.not_found_count++;
        return std::nullopt;
    }
    return it->second;
}

bool Store::erase(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    stats_.del_count++;
    bool removed = values_.erase(key) > 0;
    if (!removed) {
        stats_.not_found_count++;
    }
    stats_.keys = values_.size();
    return removed;
}

StoreStats Store::stats() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return stats_;
}

Command parse_command(const std::string& line) {
    std::istringstream input(line);
    std::string op;
    input >> op;

    if (op == "SET") {
        Command command;
        command.type = CommandType::Set;
        input >> command.key;
        std::getline(input, command.value);
        if (!command.value.empty() && command.value.front() == ' ') {
            command.value.erase(command.value.begin());
        }
        if (command.key.empty() || command.value.empty()) {
            command.type = CommandType::Invalid;
            command.error = "SET requires key and value";
        }
        return command;
    }

    if (op == "GET") {
        Command command;
        command.type = CommandType::Get;
        input >> command.key;
        if (command.key.empty()) {
            command.type = CommandType::Invalid;
            command.error = "GET requires key";
        }
        return command;
    }

    if (op == "DEL") {
        Command command;
        command.type = CommandType::Del;
        input >> command.key;
        if (command.key.empty()) {
            command.type = CommandType::Invalid;
            command.error = "DEL requires key";
        }
        return command;
    }

    if (op == "STATS") {
        return Command{CommandType::Stats, {}, {}, {}};
    }

    if (op == "QUIT") {
        return Command{CommandType::Quit, {}, {}, {}};
    }

    return Command{CommandType::Invalid, {}, {}, "unknown command"};
}

std::string execute_command(Store& store, const Command& command) {
    switch (command.type) {
    case CommandType::Set:
        store.set(command.key, command.value);
        return "OK\n";
    case CommandType::Get: {
        auto value = store.get(command.key);
        if (!value.has_value()) {
            return "NOT_FOUND\n";
        }
        return "VALUE " + *value + "\n";
    }
    case CommandType::Del:
        return store.erase(command.key) ? "DELETED\n" : "NOT_FOUND\n";
    case CommandType::Stats: {
        StoreStats stats = store.stats();
        return "STATS keys=" + std::to_string(stats.keys) +
               " set=" + std::to_string(stats.set_count) +
               " get=" + std::to_string(stats.get_count) +
               " del=" + std::to_string(stats.del_count) +
               " not_found=" + std::to_string(stats.not_found_count) + "\n";
    }
    case CommandType::Quit:
        return "BYE\n";
    case CommandType::Invalid:
        return "ERR " + command.error + "\n";
    }

    return "ERR unreachable\n";
}

} // namespace pulsekv
