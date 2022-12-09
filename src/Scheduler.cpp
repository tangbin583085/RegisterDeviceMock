#include "RegisterDeviceMock/Scheduler.hpp"

#include <limits>
#include <utility>

namespace rdm {

ScheduleResult Scheduler::schedule(Tick delay, Action action) {
    if (!action) return {Error::InvalidArgument, 0};
    if (delay > std::numeric_limits<Tick>::max() - now_) return {Error::TimeOverflow, 0};
    if (nextId_ == 0) return {Error::EventIdExhausted, 0};
    const EventId id = nextId_++;
    events_.emplace(std::make_pair(now_ + delay, id), std::move(action));
    return {Error::None, id};
}

bool Scheduler::cancel(EventId id) {
    for (auto it = events_.begin(); it != events_.end(); ++it) {
        if (it->first.second == id) {
            events_.erase(it);
            return true;
        }
    }
    return false;
}

Error Scheduler::advance(Tick delta, std::size_t maxEvents) {
    if (delta > std::numeric_limits<Tick>::max() - now_) return Error::TimeOverflow;
    const Tick target = now_ + delta;
    std::size_t executed = 0;
    while (!events_.empty() && events_.begin()->first.first <= target) {
        if (executed == maxEvents) return Error::EventLimitReached;
        auto node = events_.extract(events_.begin());
        now_ = node.key().first;
        ++executed;
        node.mapped()();
    }
    now_ = target;
    return Error::None;
}

void Scheduler::clear() noexcept {
    events_.clear();
    // Keep IDs monotonic so an old cancellation token cannot cancel a new event.
}

} // namespace rdm
