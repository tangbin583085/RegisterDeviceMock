# RegisterDeviceMock

用于模拟设备寄存器的 C++17 小型库。在没有真实设备时，可以验证寄存器读写、权限和状态变化。核心只使用标准库，可接入 Qt 或其他上位机项目。

支持按位配置访问规则，包括只读、只写、写 1 清零和读后清零。设备行为由应用程序定义，延迟动作通过手动推进模拟时间执行。

许可证：MIT。

---

## English

RegisterDeviceMock is a small C++17 library for simulating device registers. It lets you check register access, permissions and state changes without a physical device. The core uses only the standard library and can be used from Qt or other host applications.

Access rules are configurable per bit, including read-only, write-only, write-one-to-clear and clear-on-read. Applications define device behavior and advance simulated time explicitly.

License: MIT.
