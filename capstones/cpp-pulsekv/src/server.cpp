#include "pulsekv.hpp"

#include <utility>

namespace pulsekv {

UniqueFd::UniqueFd(int fd) noexcept : fd_{fd} {}

UniqueFd::~UniqueFd() {
    /* TODO */
}

UniqueFd::UniqueFd(UniqueFd&& other) noexcept : fd_{other.release()} {}

UniqueFd& UniqueFd::operator=(UniqueFd&& other) noexcept {
    if (this != &other) {
        reset(other.release());
    }
    return *this;
}

int UniqueFd::get() const noexcept {
    return fd_;
}

bool UniqueFd::valid() const noexcept {
    return fd_ >= 0;
}

int UniqueFd::release() noexcept {
    int old = fd_;
    fd_ = -1;
    return old;
}

void UniqueFd::reset(int fd) noexcept {
    (void)fd;
    /* TODO */
}

AppendLog::AppendLog(std::string path) : path_{std::move(path)} {}

bool AppendLog::open() {
    return false; /* TODO */
}

bool AppendLog::append_set(const std::string& key, const std::string& value) {
    (void)key;
    (void)value;
    return false; /* TODO */
}

bool AppendLog::append_del(const std::string& key) {
    (void)key;
    return false; /* TODO */
}

bool AppendLog::replay(Store& store) {
    (void)store;
    return false; /* TODO */
}

void AppendLog::close() noexcept {
    /* TODO */
}

ThreadPool::ThreadPool(std::size_t worker_count) {
    (void)worker_count;
    /* TODO */
}

ThreadPool::~ThreadPool() {
    stop();
}

bool ThreadPool::start(std::size_t worker_count) {
    (void)worker_count;
    return false; /* TODO */
}

bool ThreadPool::enqueue(std::function<void()> task) {
    (void)task;
    return false; /* TODO */
}

void ThreadPool::stop() {
    /* TODO */
}

Server::Server(ServerConfig config)
    : config_{std::move(config)},
      log_{config_.log_path} {}

int Server::run() {
    return -1; /* TODO */
}

void Server::request_stop() noexcept {
    running_ = false;
}

Store& Server::store() noexcept {
    return store_;
}

int run_server(const ServerConfig& config) {
    Server server{config};
    return server.run();
}

} // namespace pulsekv
