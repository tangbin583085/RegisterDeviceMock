#include "RegisterDeviceMock/Device.hpp"

#include <utility>

namespace rdm {

Error Device::onWrite(Address address, WriteHandler handler) {
    if (!bank_.contains(address)) return Error::UnknownAddress;
    if (handler) handlers_[address] = std::make_shared<WriteHandler>(std::move(handler));
    else handlers_.erase(address);
    return Error::None;
}

AccessResult Device::write(Address address, Value value) {
    // Model callbacks should update internal state rather than recursively write commands.
    if (dispatching_) return {Error::Busy};
    const auto result = bank_.write(address, value);
    const auto it = handlers_.find(address);
    if (result.ok() && it != handlers_.end()) {
        // A handler may replace or remove itself while it is running.
        const auto handler = it->second;
        struct DispatchGuard {
            bool& active;
            explicit DispatchGuard(bool& flag) : active(flag) { active = true; }
            ~DispatchGuard() { active = false; }
        } guard(dispatching_);
        (*handler)(*this, address, value, result);
    }
    return result;
}

void Device::reset() {
    scheduler_.clear();
    bank_.reset();
}

} // namespace rdm
