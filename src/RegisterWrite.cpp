#include "RegisterDeviceMock/RegisterBank.hpp"

namespace rdm {

AccessResult RegisterBank::write(Address address, Value value) {
    auto it = entries_.find(address);
    if (it == entries_.end()) return record(Operation::Write, address, value, {Error::UnknownAddress});
    auto& entry = it->second;
    const auto& definition = entry.definition;
    const Value before = entry.value;
    const Value writable = definition.access.writeMask & definition.effectiveMask();
    if (writable == 0) {
        return record(Operation::Write, address, value, {Error::WriteDenied, 0, before, before});
    }
    if ((value & ~definition.widthMask()) != 0) {
        return record(Operation::Write, address, value, {Error::InvalidValue, 0, before, before});
    }
    const Value clearBits = definition.access.writeOneToClearMask & writable;
    const Value normalBits = writable & ~clearBits;
    entry.value = ((before & ~normalBits) | (value & normalBits)) & ~(value & clearBits);
    return record(Operation::Write, address, value, {Error::None, entry.value, before, entry.value});
}

} // namespace rdm
