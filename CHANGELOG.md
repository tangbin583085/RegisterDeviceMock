# Changelog

## Unreleased

- 添加 1～32 位寄存器定义、访问掩码、复位值与定义检查。
- 支持普通读写、只读、只写、写 1 清零和读后清零，可以混合配置位权限。
- 添加内部更新接口、有容量限制的访问记录、设备写回调和模拟时间事件。
- 处理时间溢出、事件执行上限、复位取消待执行动作和回调重入。
- 保留可变回调状态，允许回调安全替换自身；异常向调用方传播。
- 提供 CMake 安装包、两个示例及测试源码。

### English

- Added 1–32-bit register definitions, masks, reset values and validation.
- Added normal, read-only, write-only, W1C and clear-on-read access with mixed bit policies.
- Added internal updates, bounded access logs, write handlers and virtual-time events.
- Handled time overflow, execution budgets, pending-action cancellation and callback reentrancy.
- Preserved mutable callback state and safe self-replacement; exceptions propagate to callers.
- Added a CMake package, two examples and test sources.
