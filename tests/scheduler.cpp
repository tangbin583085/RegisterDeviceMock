#include "RegisterDeviceMock/RegisterDeviceMock.hpp"
#include "Test.hpp"

#include <vector>

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
}

int main() { return runTests(tests); }
