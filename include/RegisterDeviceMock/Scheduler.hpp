#pragma once

#include "Types.hpp"

#include <cstddef>
#include <functional>
#include <map>
#include <utility>

namespace rdm {

struct ScheduleResult {
    Error error{Error::None};
    EventId id{0};
    bool ok() const noexcept { return error == Error::None; }
};

class Scheduler {
public:
    using Action = std::function<void()>;

    Scheduler() = default;
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(Scheduler&&) = delete;

    ScheduleResult schedule(Tick delay, Action action);
    bool cancel(EventId id);
    Error advance(Tick delta, std::size_t maxEvents = 10000);
    void clear() noexcept;

    Tick now() const noexcept { return now_; }
    std::size_t pending() const noexcept { return events_.size(); }

private:
    Tick now_{0};
    EventId nextId_{1};
    bool advancing_{false};
    std::map<std::pair<Tick, EventId>, Action> events_;
};

} // namespace rdm
