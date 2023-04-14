#pragma once

#include "RegisterBank.hpp"
#include "Scheduler.hpp"

#include <functional>
#include <map>
#include <utility>

namespace rdm {

class Device {
public:
    using WriteHandler = std::function<void(Device&, Address, Value, const AccessResult&)>;

    explicit Device(std::size_t logCapacity = 256) : bank_(logCapacity) {}
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;
    Device(Device&&) = delete;
    Device& operator=(Device&&) = delete;

    RegisterBank& registers() noexcept { return bank_; }
    const RegisterBank& registers() const noexcept { return bank_; }
    AccessResult read(Address address) { return bank_.read(address); }
    AccessResult write(Address address, Value value);
    Error onWrite(Address address, WriteHandler handler);

    ScheduleResult schedule(Tick delay, Scheduler::Action action) { return scheduler_.schedule(delay, std::move(action)); }
    bool cancel(EventId id) { return scheduler_.cancel(id); }
    Error advance(Tick delta, std::size_t maxEvents = 10000) { return scheduler_.advance(delta, maxEvents); }
    Tick now() const noexcept { return scheduler_.now(); }
    std::size_t pending() const noexcept { return scheduler_.pending(); }
    void reset();

private:
    RegisterBank bank_;
    Scheduler scheduler_;
    std::map<Address, WriteHandler> handlers_;
};

} // namespace rdm
