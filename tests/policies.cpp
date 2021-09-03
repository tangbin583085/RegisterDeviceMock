#include "RegisterDeviceMock/RegisterDeviceMock.hpp"
#include "Test.hpp"

static void tests() {
    rdm::RegisterBank bank;
    CHECK(bank.add({1, "model", 8, 0x42, 0xff, rdm::AccessPolicy::readOnly()}) == rdm::Error::None);
    CHECK(bank.write(1, 0).error == rdm::Error::WriteDenied);
    CHECK(bank.update(1, 0x43).ok());
    CHECK(bank.read(1).value == 0x43);
    CHECK(bank.add({2, "command", 8, 0, 0xff, rdm::AccessPolicy::writeOnly()}) == rdm::Error::None);
    CHECK(bank.write(2, 7).ok());
    CHECK(bank.read(2).error == rdm::Error::ReadDenied);
    CHECK(bank.add({3, "fault", 8, 0xf, 0xff, rdm::AccessPolicy::writeOneToClear()}) == rdm::Error::None);
    CHECK(bank.write(3, 5).value == 0xa);
    CHECK(bank.write(3, 0).value == 0xa);
    CHECK(bank.add({4, "event", 8, 7, 0xff, rdm::AccessPolicy::clearOnRead()}) == rdm::Error::None);
    CHECK(bank.peek(4).value == 7);
    const auto read = bank.read(4);
    CHECK(read.value == 7 && read.before == 7 && read.after == 0);
    CHECK(bank.read(4).value == 0);
    CHECK(bank.add({5, "mixed", 8, 0xc3, 0xff, {0xff, 0x0f, 0x0c, 0}}) == rdm::Error::None);
    CHECK(bank.write(5, 0x05).value == 0xc1);
    CHECK(bank.update(5, 0xcc).ok());
    CHECK(bank.write(5, 0x05).value == 0xc9);
}

int main() { return runTests(tests); }
