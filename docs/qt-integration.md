# Qt 项目接入

核心没有 QObject 或 GUI 对象。在已有 Qt CMake 工程中加入源码目录，然后把 `RDM::Core` 链接到你的目标即可；Qt Widgets、Qt Quick 都可以使用。

```cmake
add_subdirectory(path/to/RegisterDeviceMock)
target_link_libraries(your_qt_app PRIVATE RDM::Core)
```

建议把 `rdm::Device` 放在一个由你管理的模型对象中，在同一线程里读写。界面显示时检查 `AccessResult.error`，不要用零值代替失败结果。直接定期调用 `advance(10)` 表示模拟时间前进 10 tick，不代表真实世界一定过去了 10 毫秒。

如果用 QTimer 驱动模型，可根据 QElapsedTimer 的增量推进，或明确使用固定步长。定时器延迟触发时两者的行为不同，需要按测试目的选择。处理 `EventLimitReached` 时使用当前 `now()` 计算尚未完成的时间，避免重复推进。

串口、TCP、Modbus 等适配层负责解码地址和数值、处理字节序，再调用 `Device::read()` 或 `Device::write()`。内部 `peek()` 和 `update()` 用于模拟设备自身，不应作为外部读写权限的替代接口。

## English

The core contains no QObject or GUI objects. Add its source directory and link `RDM::Core` to an existing Qt Widgets or Qt Quick target, as shown above.

Own `rdm::Device` in your application model and access it from one thread. Check result errors before updating UI values. Calling `advance(10)` advances ten simulated ticks; ticks are not implicitly milliseconds.

A QTimer can drive the model using a fixed step or elapsed-time increments from QElapsedTimer. Choose deliberately: delayed timer delivery has different effects in each approach. When an event budget is reached, calculate the remaining interval from `now()` before continuing.

The transport/protocol adapter decodes addresses and values, handles byte order, and calls public device read/write operations. Keep permission-bypassing internal inspection and updates inside the simulated device model.
