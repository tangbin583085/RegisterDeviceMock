#include <RegisterDeviceMock/RegisterDeviceMock.hpp>

int main() {
    rdm::RegisterBank bank;
    if (bank.add({1, "value", 16, 42}) != rdm::Error::None) return 1;
    const auto result = bank.read(1);
    return result.ok() && result.value == 42 ? 0 : 1;
}
