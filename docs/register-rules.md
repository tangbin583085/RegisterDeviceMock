# 寄存器规则

地址为 32 位无符号数，由应用决定表示寄存器编号还是字节地址。本库不推断地址步长、不处理对齐和字节序。

- 位宽为 1～32；`implementedMask` 会与位宽掩码相交，交集不能为零。
- 复位值不能超出有效位。访问策略掩码在位宽和有效位之外的部分会被忽略，方便使用默认的全位掩码。
- `readMask`、`writeMask` 分别控制可读和可写位，可以在同一寄存器内混合使用。
- 写入超过位宽的数据返回 `InvalidValue`；位宽内的只读位、保留位写入被忽略。
- W1C 位必须同时可写，读后清零位必须同时可读，否则定义无效。
- 写 1 清零：输入位为 1 时清除，输入位为 0 时保留。普通可写位按输入值替换。
- 读后清零：先返回清除前的可读值，再清除配置的位，其他位保留。
- 完全不可读或不可写时分别返回 `ReadDenied`、`WriteDenied`，寄存器值不变。

`peek()` 用于设备模型内部检查，不触发读后清零、不写访问记录；`update()` 绕过外部读写权限，按掩码更新内部状态并记录访问。

`AccessResult.value` 在读取时是屏蔽后的读数，写入或内部更新时是更新后的完整内部值。`before`、`after` 是完整内部快照，供测试调试使用；不要将它们直接作为面向真实设备客户端的协议响应。失败时请先检查 `error`，不要把默认值当作有效读数。

访问记录容量默认 256，满后移除最旧记录；容量设为 0 可关闭记录。记录含操作、地址、输入、结果及前后值，不包含墙上时钟时间。`reset()` 恢复复位值并清空访问记录。

## English

Addresses are unsigned 32-bit values. Your adapter decides whether they represent byte offsets or register numbers. The core does not infer alignment, stride or endianness.

Widths range from 1 to 32 bits. The implemented mask is intersected with the width mask and must leave at least one bit. Reset values must fit the effective mask. Policy bits outside the effective mask are ignored, allowing full-width default policies on narrow registers.

Readable and writable masks are independent. W1C bits must also be writable; clear-on-read bits must also be readable. A write replaces normal writable bits and clears only W1C bits whose input is one. Read-only and reserved bits are preserved; inputs outside the register width are rejected. Reads return the masked value before clearing configured readable bits.

`peek()` returns internal state without permissions, side effects or logging. `update()` applies a masked internal update without access restrictions and records it. Entirely unreadable or unwritable registers reject external access without changing state.

For a read, `AccessResult.value` is the readable value; for a write or internal update it is the full resulting state. `before` and `after` expose internal snapshots for diagnostics, including non-readable bits. Do not use these snapshots as protocol responses to external clients. Check `error` before using a result.

The log holds 256 records by default, evicts the oldest entry at capacity, and can be disabled with capacity zero. Records contain operation, address, input and result snapshots; they do not contain wall-clock timestamps. Reset restores register values and clears the log.
