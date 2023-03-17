#include "RegisterDeviceMock/Device.hpp"

#include <utility>

namespace rdm {

Error Device::onWrite(Address address, WriteHandler handler) {
    if (!bank_.contains(address)) return Error::UnknownAddress;
    if (handler) handlers_[address] = std::move(handler);
    else handlers_.erase(address);
    return Error::None;
}

AccessResult Device::write(Address address, Value value) {
    const auto result = bank_.write(address, value);
    const auto it = handlers_.find(address);
    if (result.ok() && it != handlers_.end()) {
        // A handler may replace or remove itself while it is running.
        const auto handler = it->second;
        handler(*this, address, value, result);
    }
    return result;
}

void Device::reset() {
    scheduler_.clear();
    bank_.reset();
}

} // namespace rdm
