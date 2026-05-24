#include "raii.hpp"

#include <unistd.h>

#include <utility>

namespace training {

UniqueFd::UniqueFd(int fd) noexcept : fd_{fd} {}

UniqueFd::~UniqueFd() {
    reset();
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
    if (fd_ >= 0) {
        (void)::close(fd_);
    }
    fd_ = fd;
}

OwnedBuffer::OwnedBuffer(std::size_t size) : bytes_(size) {}

std::size_t OwnedBuffer::size() const noexcept {
    return bytes_.size();
}

unsigned char* OwnedBuffer::data() noexcept {
    return bytes_.data();
}

const unsigned char* OwnedBuffer::data() const noexcept {
    return bytes_.data();
}

void OwnedBuffer::append(const unsigned char* bytes, std::size_t count) {
    (void)bytes;
    (void)count;
    /* TODO */
}

std::string OwnedBuffer::as_string() const {
    return {}; /* TODO */
}

std::optional<std::string> read_file(const std::string& path) {
    (void)path;
    return std::nullopt; /* TODO */
}

bool write_file(const std::string& path, const std::string& contents) {
    (void)path;
    (void)contents;
    return false; /* TODO */
}

CommandResult run_command_capture_status(const std::vector<std::string>& argv) {
    (void)argv;
    return {}; /* TODO */
}

} // namespace training
