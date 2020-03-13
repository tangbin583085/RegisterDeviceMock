#include "RegisterDeviceMock/RegisterBank.hpp"

namespace rdm {

AccessResult RegisterBank::update(Address address, Value value, Value mask) {
    auto it = entries_.find(address);
    if (it == entries_.end()) return record(Operation::InternalUpdate, address, value, {Error::UnknownAddress});
    auto& entry = it->second;
    const Value before = entry.value;
    if ((value & ~entry.definition.widthMask()) != 0) {
        return record(Operation::InternalUpdate, address, value, {Error::InvalidValue, 0, before, before});
    }
    mask &= entry.definition.effectiveMask();
    entry.value = (before & ~mask) | (value & mask);
    return record(Operation::InternalUpdate, address, value, {Error::None, entry.value, before, entry.value});
}

void RegisterBank::reset() noexcept {
    for (auto& pair : entries_) pair.second.value = pair.second.definition.resetValue;
    log_.clear();
}

} // namespace rdm
