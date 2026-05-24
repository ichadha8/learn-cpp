#ifndef TRAINING_CAPSTONE_PULSEKV_HPP
#define TRAINING_CAPSTONE_PULSEKV_HPP

#include <condition_variable>
#include <cstdint>
#include <cstddef>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace pulsekv {

enum class CommandType {
    Invalid,
    Set,
    Get,
    Del,
    Stats,
    Ping,
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
    std::uint64_t command_count{0};
    std::uint64_t set_count{0};
    std::uint64_t get_count{0};
    std::uint64_t del_count{0};
    std::uint64_t not_found_count{0};
    std::uint64_t active_clients{0};
    std::uint64_t bytes_read{0};
    std::uint64_t bytes_written{0};
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

class UniqueFd {
public:
    UniqueFd() noexcept = default;
    explicit UniqueFd(int fd) noexcept;
    ~UniqueFd();

    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;

    UniqueFd(UniqueFd&& other) noexcept;
    UniqueFd& operator=(UniqueFd&& other) noexcept;

    [[nodiscard]] int get() const noexcept;
    [[nodiscard]] bool valid() const noexcept;
    int release() noexcept;
    void reset(int fd = -1) noexcept;

private:
    int fd_{-1};
};

class AppendLog {
public:
    AppendLog() = default;
    explicit AppendLog(std::string path);

    [[nodiscard]] bool open();
    [[nodiscard]] bool append_set(const std::string& key, const std::string& value);
    [[nodiscard]] bool append_del(const std::string& key);
    [[nodiscard]] bool replay(Store& store);
    void close() noexcept;

private:
    std::string path_;
    UniqueFd fd_;
};

class ThreadPool {
public:
    ThreadPool() = default;
    explicit ThreadPool(std::size_t worker_count);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    [[nodiscard]] bool start(std::size_t worker_count);
    [[nodiscard]] bool enqueue(std::function<void()> task);
    void stop();

private:
    std::vector<std::thread> workers_;
    std::deque<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopping_{false};
};

struct ServerConfig {
    std::uint16_t port{9090};
    std::size_t worker_count{4};
    std::string log_path;
    bool enable_persistence{false};
};

class Server {
public:
    explicit Server(ServerConfig config);

    [[nodiscard]] int run();
    void request_stop() noexcept;
    [[nodiscard]] Store& store() noexcept;

private:
    ServerConfig config_;
    Store store_;
    AppendLog log_;
    ThreadPool workers_;
    bool running_{false};
};

Command parse_command(const std::string& line);
std::string execute_command(Store& store, const Command& command);
std::string format_stats(const StoreStats& stats);
bool is_valid_key(const std::string& key);
int run_server(const ServerConfig& config);

} // namespace pulsekv

#endif
