#ifndef TRAINING_CAPSTONE_PULSEKV_HPP
#define TRAINING_CAPSTONE_PULSEKV_HPP

#include <cstdint>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

namespace pulsekv {

enum class CommandType {
    Invalid,
    Set,
    Get,
    Del,
    Stats,
    Quit
};

struct Command {
    CommandType type{CommandType::Invalid};
    std::string key;
    std::string value;
    std::string error;
};

struct StoreStats {
    std::uint64_t keys{0};
    std::uint64_t set_count{0};
    std::uint64_t get_count{0};
    std::uint64_t del_count{0};
    std::uint64_t not_found_count{0};
};

class Store {
public:
    void set(std::string key, std::string value);
    std::optional<std::string> get(const std::string& key);
    bool erase(const std::string& key);
    StoreStats stats() const;

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::string> values_;
    StoreStats stats_;
};

Command parse_command(const std::string& line);
std::string execute_command(Store& store, const Command& command);

} // namespace pulsekv

#endif
