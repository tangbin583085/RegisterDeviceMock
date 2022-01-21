#include <RegisterDeviceMock/RegisterDeviceMock.hpp>

#include <iostream>

int main() {
    rdm::RegisterBank bank;
    if (bank.add({0x10, "control", 8, 0}) != rdm::Error::None ||
        bank.add({0x20, "fault", 8, 0x0f, 0xff, rdm::AccessPolicy::writeOneToClear()}) != rdm::Error::None) {
        return 1;
    }
    if (!bank.write(0x10, 3) || !bank.write(0x20, 5)) return 1;
    const auto control = bank.read(0x10);
    const auto fault = bank.read(0x20);
    if (!control || !fault) return 1;
    std::cout << "control=" << control.value << ", fault=" << fault.value << '\n';
    return 0;
}
