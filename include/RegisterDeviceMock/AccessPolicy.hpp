#pragma once

#include "Types.hpp"

namespace rdm {

struct AccessPolicy {
    Value readMask{0xffffffffu};
    Value writeMask{0xffffffffu};
    Value writeOneToClearMask{0};
    Value readClearMask{0};

    static AccessPolicy readWrite(Value mask = 0xffffffffu) noexcept {
        return {mask, mask, 0, 0};
    }
    static AccessPolicy readOnly(Value mask = 0xffffffffu) noexcept {
        return {mask, 0, 0, 0};
    }
    static AccessPolicy writeOnly(Value mask = 0xffffffffu) noexcept {
        return {0, mask, 0, 0};
    }
    static AccessPolicy writeOneToClear(Value mask = 0xffffffffu) noexcept {
        return {mask, mask, mask, 0};
    }
    static AccessPolicy clearOnRead(Value mask = 0xffffffffu) noexcept {
        return {mask, 0, 0, mask};
    }
};

} // namespace rdm
