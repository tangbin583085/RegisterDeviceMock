# RegisterDeviceMock

用于模拟设备寄存器的 C++17 小型库。在没有真实设备时，可以验证寄存器读写、权限和状态变化。核心只使用标准库，可接入 Qt 或其他上位机项目。

支持按位配置访问规则，包括只读、只写、写 1 清零和读后清零。设备行为由应用程序定义，延迟动作通过手动推进模拟时间执行。

### 集成

需要 C++17 和 CMake 3.16+，不依赖 Qt。放进现有 CMake 工程后：

```cmake
add_subdirectory(path/to/RegisterDeviceMock)
target_link_libraries(your_app PRIVATE RDM::Core)
```

也可以安装后使用 `find_package(RegisterDeviceMock 0.1 CONFIG REQUIRED)`。完整构建、安装和测试命令见 [构建说明](docs/building.md)。

### 简单使用

```cpp
#include <RegisterDeviceMock/RegisterDeviceMock.hpp>
#include <iostream>

int main() {
    rdm::RegisterBank bank;
    if (bank.add({0x20, "fault", 8, 0x0f, 0xff,
                  rdm::AccessPolicy::writeOneToClear()}) != rdm::Error::None) {
        return 1;
    }
    if (!bank.write(0x20, 0x05)) return 1;
    const auto result = bank.read(0x20);
    if (!result) return 1;
    std::cout << result.value << '\n'; // 10 (0x0a)
}
```

如果需要模拟“启动后进入忙状态，过一段时间完成”，使用 `rdm::Device` 配置写回调，并用 `schedule()`、`advance()` 控制时间。见 [设备行为示例](examples/simulated_device/main.cpp)。时间单位由你统一约定，不会让线程真实等待。

### 使用时注意

- 寄存器支持 1～32 位，地址由你定义，不自动处理地址步长或字节序。
- 读取返回清除前的可读值，再执行读后清零；写 1 清零只影响写入为 1 的对应位。
- 用 `peek()` 检查内部值，不触发读副作用；用 `update()` 修改设备内部状态。
- `reset()` 恢复复位值、清空访问记录并取消待执行动作，保留已配置的行为和当前模拟时间。
- 按单线程使用，需要并发时由调用方同步。设备协议的编码、串口或 TCP 接入放在应用适配层。

详细说明：[寄存器规则](docs/register-rules.md)、[设备行为](docs/device-behavior.md)。

许可证：MIT。

---

## English

RegisterDeviceMock is a small C++17 library for simulating device registers. It lets you check register access, permissions and state changes without a physical device. The core uses only the standard library and can be used from Qt or other host applications.

Access rules are configurable per bit, including read-only, write-only, write-one-to-clear and clear-on-read. Applications define device behavior and advance simulated time explicitly.

### Integration

Use C++17 and CMake 3.16 or newer. Qt is optional. Add the source directory to your CMake project:

```cmake
add_subdirectory(path/to/RegisterDeviceMock)
target_link_libraries(your_app PRIVATE RDM::Core)
```

For an installed package, use `find_package(RegisterDeviceMock 0.1 CONFIG REQUIRED)` and link the same target. See [build instructions](docs/building.md#english) for build, install and test commands.

### Basic use

```cpp
#include <RegisterDeviceMock/RegisterDeviceMock.hpp>
#include <iostream>

int main() {
    rdm::RegisterBank bank;
    if (bank.add({0x20, "fault", 8, 0x0f, 0xff,
                  rdm::AccessPolicy::writeOneToClear()}) != rdm::Error::None) {
        return 1;
    }
    if (!bank.write(0x20, 0x05)) return 1;
    const auto result = bank.read(0x20);
    if (!result) return 1;
    std::cout << result.value << '\n'; // 10 (0x0a)
}
```

Use `rdm::Device` when a write should trigger behavior. A handler can update a status register and schedule a completion action. Call `advance()` to move virtual time forward; no thread sleeps are involved. See the [simulated device example](examples/simulated_device/main.cpp).

### Usage notes

- Registers have a width of 1–32 bits. Addresses, alignment and byte order belong to your device model and adapter.
- A read returns readable bits before applying clear-on-read. A write-one-to-clear bit changes only when its input bit is one.
- Use `peek()` for side-effect-free internal inspection and `update()` for device-owned state changes.
- `reset()` restores reset values, clears access records and cancels pending actions. Handlers and current simulated time are retained.
- Use the library from one thread, or synchronize access externally. Protocol encoding and serial/TCP access belong in an application adapter.

More details: [register rules](docs/register-rules.md#english), [device behavior](docs/device-behavior.md#english).

License: MIT.
