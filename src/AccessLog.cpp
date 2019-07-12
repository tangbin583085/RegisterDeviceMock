#include "RegisterDeviceMock/AccessLog.hpp"

namespace rdm {

void AccessLog::append(const AccessRecord& record) {
    if (capacity_ == 0) return;
    records_.push_back(record);
    while (records_.size() > capacity_) records_.pop_front();
}

void AccessLog::setCapacity(std::size_t capacity) {
    capacity_ = capacity;
    while (records_.size() > capacity_) records_.pop_front();
}

void AccessLog::clear() noexcept {
    records_.clear();
}

} // namespace rdm
