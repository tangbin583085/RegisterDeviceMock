#include "RegisterDeviceMock/RegisterDefinition.hpp"

namespace rdm {

Value RegisterDefinition::widthMask() const noexcept {
    if (width == 0 || width > 32) return 0;
    return width == 32 ? 0xffffffffu : (Value{1} << width) - 1u;
}

Value RegisterDefinition::effectiveMask() const noexcept {
    return widthMask() & implementedMask;
}

Error RegisterDefinition::validate() const noexcept {
    const Value mask = effectiveMask();
    if (width == 0 || width > 32 || mask == 0 || (resetValue & ~mask) != 0) {
        return Error::InvalidDefinition;
    }
    if ((access.writeOneToClearMask & mask & ~access.writeMask) != 0 ||
        (access.readClearMask & mask & ~access.readMask) != 0) {
        return Error::InvalidDefinition;
    }
    return Error::None;
}

} // namespace rdm
