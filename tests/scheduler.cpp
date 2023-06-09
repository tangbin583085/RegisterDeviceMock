#include "RegisterDeviceMock/RegisterDeviceMock.hpp"
#include "Test.hpp"

#include <vector>
#include <limits>
#include <stdexcept>

static void tests() {
    rdm::Scheduler clock;
    std::vector<int> order;
    CHECK(clock.schedule(10, [&] { order.push_back(1); }).ok());
    CHECK(clock.schedule(10, [&] { order.push_back(2); }).ok());
    const auto cancelled = clock.schedule(5, [&] { order.push_back(0); });
    CHECK(clock.cancel(cancelled.id));
    CHECK(!clock.cancel(cancelled.id));
    CHECK(clock.advance(9) == rdm::Error::None);
    CHECK(order.empty());
    CHECK(clock.advance(1) == rdm::Error::None);
    CHECK(order == std::vector<int>({1, 2}));
    CHECK(clock.now() == 10 && clock.pending() == 0);
    CHECK(clock.schedule(0, {}).error == rdm::Error::InvalidArgument);

    CHECK(clock.schedule(0, [&] {
        CHECK(clock.advance(1) == rdm::Error::Busy);
        CHECK(clock.schedule(0, [&] { order.push_back(3); }).ok());
    }).ok());
    CHECK(clock.advance(0, 1) == rdm::Error::EventLimitReached);
    CHECK(clock.now() == 10 && clock.pending() == 1);
    CHECK(clock.advance(0) == rdm::Error::None);
    CHECK(order.back() == 3);

    CHECK(clock.schedule(0, [] { throw std::runtime_error("callback failure"); }).ok());
    bool threw = false;
    try { (void)clock.advance(0); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
    CHECK(clock.pending() == 0);
    CHECK(clock.advance(1) == rdm::Error::None);

    rdm::Scheduler boundary;
    CHECK(boundary.advance(std::numeric_limits<rdm::Tick>::max()) == rdm::Error::None);
    CHECK(boundary.schedule(1, [] {}).error == rdm::Error::TimeOverflow);
    CHECK(boundary.advance(1) == rdm::Error::TimeOverflow);
    CHECK(boundary.schedule(0, [] {}).ok());
    CHECK(boundary.advance(0) == rdm::Error::None);

    const auto old = clock.schedule(0, [] {});
    clock.clear();
    const auto fresh = clock.schedule(0, [] {});
    CHECK(old.id != fresh.id);
    CHECK(!clock.cancel(old.id));
    CHECK(clock.cancel(fresh.id));
}

int main() { return runTests(tests); }
