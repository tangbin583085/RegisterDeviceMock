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
}

int main() { return runTests(tests); }
