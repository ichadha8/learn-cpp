#ifndef TRAINING_HW04_RAII_HPP
#define TRAINING_HW04_RAII_HPP

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace training {

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

class OwnedBuffer {
public:
    OwnedBuffer() = default;
    explicit OwnedBuffer(std::size_t size);

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] unsigned char* data() noexcept;
    [[nodiscard]] const unsigned char* data() const noexcept;

    void append(const unsigned char* bytes, std::size_t count);
    [[nodiscard]] std::string as_string() const;

private:
    std::vector<unsigned char> bytes_;
};

struct CommandResult {
    int exit_code{-1};
    bool signaled{false};
    int signal_number{0};
};

std::optional<std::string> read_file(const std::string& path);
bool write_file(const std::string& path, const std::string& contents);
CommandResult run_command_capture_status(const std::vector<std::string>& argv);

} // namespace training

#endif
