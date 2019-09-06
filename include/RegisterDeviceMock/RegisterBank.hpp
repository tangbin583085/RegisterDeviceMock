#pragma once

#include "AccessLog.hpp"
#include "RegisterDefinition.hpp"

#include <map>

namespace rdm {

class RegisterBank {
public:
    explicit RegisterBank(std::size_t logCapacity = 256) : log_(logCapacity) {}

    Error add(RegisterDefinition definition);
    bool contains(Address address) const noexcept;
    std::size_t size() const noexcept { return entries_.size(); }
    const RegisterDefinition* definition(Address address) const noexcept;

    AccessResult read(Address address);
    AccessResult write(Address address, Value value);
    // Internal inspection bypasses access permissions and has no read side effects.
    AccessResult peek(Address address) const noexcept;
    // Internal updates bypass access permissions and preserve bits outside the mask.
    AccessResult update(Address address, Value value, Value mask = 0xffffffffu);
    void reset() noexcept;

    AccessLog& log() noexcept { return log_; }
    const AccessLog& log() const noexcept { return log_; }

private:
    struct Entry {
        RegisterDefinition definition;
        Value value;
    };

    AccessResult record(Operation operation, Address address, Value input, AccessResult result);
    std::map<Address, Entry> entries_;
    AccessLog log_;
};

} // namespace rdm
