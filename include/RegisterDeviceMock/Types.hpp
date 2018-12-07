#pragma once

#include <cstdint>

namespace rdm {

using Address = std::uint32_t;
using Value = std::uint32_t;
using Tick = std::uint64_t;
using EventId = std::uint64_t;

enum class Error {
    None,
    InvalidDefinition,
    DuplicateAddress,
    UnknownAddress,
    ReadDenied,
    WriteDenied,
    InvalidValue,
    InvalidArgument,
    TimeOverflow,
    EventIdExhausted,
    EventLimitReached,
    Busy
};

inline const char* errorName(Error error) noexcept {
    switch (error) {
    case Error::None: return "none";
    case Error::InvalidDefinition: return "invalid definition";
    case Error::DuplicateAddress: return "duplicate address";
    case Error::UnknownAddress: return "unknown address";
    case Error::ReadDenied: return "read denied";
    case Error::WriteDenied: return "write denied";
    case Error::InvalidValue: return "value outside register width";
    case Error::InvalidArgument: return "invalid argument";
    case Error::TimeOverflow: return "time overflow";
    case Error::EventIdExhausted: return "event id exhausted";
    case Error::EventLimitReached: return "event limit reached";
    case Error::Busy: return "operation is already active";
    }
    return "unknown error";
}

struct AccessResult {
    Error error{Error::None};
    Value value{0};
    Value before{0};
    Value after{0};

    bool ok() const noexcept { return error == Error::None; }
    explicit operator bool() const noexcept { return ok(); }
};

} // namespace rdm
