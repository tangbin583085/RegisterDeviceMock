#include "RegisterDeviceMock/RegisterDeviceMock.hpp"
#include "Test.hpp"

static void tests() {
    rdm::RegisterBank bank;
    CHECK(bank.add({0x10, "control", 8, 3}) == rdm::Error::None);
    CHECK(bank.add({0x10, "duplicate"}) == rdm::Error::DuplicateAddress);
    CHECK(bank.size() == 1);
    CHECK(bank.read(0x10).value == 3);
    CHECK(bank.write(0x10, 7).ok());
    CHECK(bank.peek(0x10).value == 7);
    CHECK(bank.write(0x10, 0x100).error == rdm::Error::InvalidValue);
    CHECK(bank.peek(0x10).value == 7);
    CHECK(bank.read(0xffff).error == rdm::Error::UnknownAddress);
    CHECK(bank.write(0xffff, 0).error == rdm::Error::UnknownAddress);
    CHECK(bank.update(0x10, 0, 2).value == 5);
    bank.reset();
    CHECK(bank.peek(0x10).value == 3);
    CHECK(bank.log().records().empty());
    CHECK(bank.add({0xffffffffu, "last address", 32, 0xffffffffu}) == rdm::Error::None);
    CHECK(bank.read(0xffffffffu).value == 0xffffffffu);
    CHECK(bank.add({0x20, "reserved", 8, 0xa, 0x0f}) == rdm::Error::None);
    CHECK(bank.write(0x20, 0xf5).value == 5);
    CHECK(bank.update(0x20, 8, 8).value == 13);
    CHECK(bank.update(0x20, 0x100).error == rdm::Error::InvalidValue);
    CHECK(bank.peek(0x20).value == 13);
    CHECK(bank.update(0x30, 0).error == rdm::Error::UnknownAddress);
    bank.log().setCapacity(2);
    (void)bank.read(0x10);
    (void)bank.read(0x20);
    (void)bank.read(0x30);
    CHECK(bank.log().records().size() == 2);
    CHECK(bank.log().records().front().address == 0x20);
    CHECK(bank.log().records().back().result.error == rdm::Error::UnknownAddress);
    bank.log().setCapacity(0);
    (void)bank.read(0x10);
    CHECK(bank.log().records().empty());
}

int main() { return runTests(tests); }
