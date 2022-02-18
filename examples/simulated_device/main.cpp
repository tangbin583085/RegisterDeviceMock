#include <RegisterDeviceMock/RegisterDeviceMock.hpp>

#include <iostream>
#include <stdexcept>

static void require(rdm::Error error) {
    if (error != rdm::Error::None) throw std::runtime_error(rdm::errorName(error));
}

int main() {
    try {
        rdm::Device device;
        auto& bank = device.registers();
        require(bank.add({0x10, "control", 8, 0}));
        require(bank.add({0x11, "status", 8, 0, 0xff, rdm::AccessPolicy::readOnly()}));
        require(bank.add({0x30, "event", 8, 0, 0xff, rdm::AccessPolicy::clearOnRead()}));

        require(device.onWrite(0x10, [](rdm::Device& dev, rdm::Address, rdm::Value input, const rdm::AccessResult&) {
            if ((input & 1u) == 0 || dev.registers().peek(0x11).value == 1) return;
            require(dev.registers().update(0x11, 1).error);
            const auto pending = dev.schedule(100, [&dev] {
                require(dev.registers().update(0x11, 2).error);
                require(dev.registers().update(0x30, 1, 1).error);
            });
            if (!pending.ok()) {
                require(dev.registers().update(0x11, 0).error);
                require(pending.error);
            }
        }));

        require(device.write(0x10, 1).error);
        std::cout << "busy=" << device.read(0x11).value << '\n';
        require(device.advance(100)); // This example treats one tick as one millisecond.
        std::cout << "done=" << device.read(0x11).value << '\n';
        std::cout << "event=" << device.read(0x30).value << '\n';
        std::cout << "event after read=" << device.read(0x30).value << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
