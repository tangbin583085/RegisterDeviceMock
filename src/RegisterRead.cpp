#include "RegisterDeviceMock/RegisterBank.hpp"

namespace rdm {

AccessResult RegisterBank::peek(Address address) const noexcept {
    const auto it = entries_.find(address);
    if (it == entries_.end()) return {Error::UnknownAddress};
    const Value value = it->second.value;
    return {Error::None, value, value, value};
}

AccessResult RegisterBank::read(Address address) {
    auto it = entries_.find(address);
    if (it == entries_.end()) return record(Operation::Read, address, 0, {Error::UnknownAddress});
    auto& entry = it->second;
    const auto& definition = entry.definition;
    const Value before = entry.value;
    const Value readable = definition.access.readMask & definition.effectiveMask();
    if (readable == 0) {
        return record(Operation::Read, address, 0, {Error::ReadDenied, 0, before, before});
    }
    const Value result = before & readable;
    entry.value &= ~(definition.access.readClearMask & readable);
    return record(Operation::Read, address, 0, {Error::None, result, before, entry.value});
}

} // namespace rdm
