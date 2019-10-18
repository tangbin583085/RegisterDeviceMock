#include "RegisterDeviceMock/RegisterBank.hpp"

#include <utility>

namespace rdm {

Error RegisterBank::add(RegisterDefinition definition) {
    if (definition.validate() != Error::None) return Error::InvalidDefinition;
    if (contains(definition.address)) return Error::DuplicateAddress;
    const Address address = definition.address;
    const Value initial = definition.resetValue;
    entries_.emplace(address, Entry{std::move(definition), initial});
    return Error::None;
}

bool RegisterBank::contains(Address address) const noexcept {
    return entries_.find(address) != entries_.end();
}

const RegisterDefinition* RegisterBank::definition(Address address) const noexcept {
    const auto it = entries_.find(address);
    return it == entries_.end() ? nullptr : &it->second.definition;
}

AccessResult RegisterBank::record(Operation operation, Address address, Value input, AccessResult result) {
    log_.append({operation, address, input, result});
    return result;
}

} // namespace rdm
