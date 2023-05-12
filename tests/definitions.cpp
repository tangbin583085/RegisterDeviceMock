#include "RegisterDeviceMock/RegisterDeviceMock.hpp"
#include "Test.hpp"

static void tests() {
    rdm::RegisterDefinition def;
    CHECK(def.validate() == rdm::Error::None);
    CHECK(def.widthMask() == 0xffffffffu);
    def.width = 8;
    CHECK(def.effectiveMask() == 0xffu);
    def.resetValue = 0x100;
    CHECK(def.validate() == rdm::Error::InvalidDefinition);
    def.resetValue = 0;
    def.width = 0;
    CHECK(def.validate() == rdm::Error::InvalidDefinition);
    def.width = 33;
    CHECK(def.widthMask() == 0);
    def.width = 8;
    def.access = {0xff, 0, 1, 0};
    CHECK(def.validate() == rdm::Error::InvalidDefinition);
    def.access = {0, 0xff, 0, 1};
    CHECK(def.validate() == rdm::Error::InvalidDefinition);
    def = {};
    def.width = 1;
    def.resetValue = 1;
    CHECK(def.validate() == rdm::Error::None);
    def.width = 31;
    CHECK(def.widthMask() == 0x7fffffffu);
    def.implementedMask = 0;
    CHECK(def.validate() == rdm::Error::InvalidDefinition);
}

int main() { return runTests(tests); }
