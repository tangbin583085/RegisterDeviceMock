#pragma once

#include "AccessPolicy.hpp"

#include <string>

namespace rdm {

struct RegisterDefinition {
    Address address{0};
    std::string name;
    unsigned width{32};
    Value resetValue{0};
    Value implementedMask{0xffffffffu};
    AccessPolicy access{};

    Value widthMask() const noexcept;
    Value effectiveMask() const noexcept;
    Error validate() const noexcept;
};

} // namespace rdm
