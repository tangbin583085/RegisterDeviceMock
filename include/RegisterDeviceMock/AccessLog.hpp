#pragma once

#include "Types.hpp"

#include <cstddef>
#include <deque>

namespace rdm {

enum class Operation { Read, Write, InternalUpdate };

struct AccessRecord {
    Operation operation{Operation::Read};
    Address address{0};
    Value input{0};
    AccessResult result{};
};

class AccessLog {
public:
    explicit AccessLog(std::size_t capacity = 256) : capacity_(capacity) {}

    void append(const AccessRecord& record);
    void setCapacity(std::size_t capacity);
    void clear() noexcept;
    std::size_t capacity() const noexcept { return capacity_; }
    const std::deque<AccessRecord>& records() const noexcept { return records_; }

private:
    std::size_t capacity_;
    std::deque<AccessRecord> records_;
};

} // namespace rdm
