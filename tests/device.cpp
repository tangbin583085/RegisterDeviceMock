#include "RegisterDeviceMock/RegisterDeviceMock.hpp"
#include "Test.hpp"

static void tests() {
    rdm::Device device;
    CHECK(device.registers().add({1, "command"}) == rdm::Error::None);
    CHECK(device.registers().add({2, "status", 8, 0, 0xff, rdm::AccessPolicy::readOnly()}) == rdm::Error::None);
    CHECK(device.onWrite(1, [](rdm::Device& dev, rdm::Address, rdm::Value input, const rdm::AccessResult&) {
        if (input == 1) {
            CHECK(dev.registers().update(2, 1).ok());
            CHECK(dev.schedule(10, [&dev] { CHECK(dev.registers().update(2, 2).ok()); }).ok());
        }
    }) == rdm::Error::None);
    CHECK(device.write(1, 1).ok());
    CHECK(device.read(2).value == 1);
    CHECK(device.advance(10) == rdm::Error::None);
    CHECK(device.read(2).value == 2);
    device.reset();
    CHECK(device.read(2).value == 0);
    CHECK(device.onWrite(99, {}) == rdm::Error::UnknownAddress);
}

int main() { return runTests(tests); }
