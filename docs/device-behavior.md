# 设备行为与模拟时间

`Device::onWrite()` 为指定地址配置一个回调。只有写操作成功才调用，回调收到原始输入以及写操作完成时的结果快照。再次注册会替换旧回调，传空回调可移除。直接调用 `registers().write()` 不触发设备回调，适合需要自行控制行为的场合。

模型内部通过 `registers().update()` 更新状态和事件位。读操作本身不触发用户回调。

`schedule(delay, action)` 以相对当前时间的 tick 安排动作；tick 单位由应用统一约定，不使用真实等待。`advance(delta)` 按到期时间顺序执行动作；同一到期时间按注册顺序执行。零延迟动作在下一次 `advance()`（包括当前正在执行的推进过程）中执行，不在注册时立即执行。

每次推进默认最多执行 10000 个事件。触及上限返回 `EventLimitReached`，尚未执行的事件保留，时间停在最后执行事件的时刻；调用方应重新计算剩余时间，不能把原 delta 直接重复使用。事件 ID 只用于取消，不应假定连续或在清空后重用。

库按单线程使用设计；Qt 应在同一线程调用，或由外部加锁。回调捕获的对象必须活到动作执行或取消。回调异常向调用方传播，已经发生的寄存器变更不回滚，已经取出的事件不会自动重试。

`AccessResult` 描述写入本身的结果；回调可能随后改变设备状态，应重新读取需要观察的状态寄存器。模型应通过单独的状态/故障寄存器表达“忙”“执行失败”等业务结果。

回调执行期间不允许嵌套 `Device::write()`，返回 `Busy`，不会修改寄存器或记录这次被拦截的写入；内部状态应使用 `update()`。回调可安全替换/移除自身，未被替换的可变回调状态会跨写入保留。异常离开时重入保护也会解除。

`reset()` 取消所有待执行事件，恢复寄存器并清空日志；保留回调和当前时间，不重用旧事件 ID。`advance()` 不能嵌套调用，时间加法溢出会在执行前返回错误。`Device` 和 `Scheduler` 不可复制或移动，防止捕获旧对象引用或复制正在执行的状态。

## English

Register a single handler per address with `Device::onWrite()`. Successful device writes invoke it with the raw input and the completed write snapshot. Registering another handler replaces it; an empty handler removes it. Calling `registers().write()` directly bypasses device handlers. Reads have no user callback.

Use internal `update()` calls for device-owned state. Schedule actions relative to current virtual time and advance time explicitly. Ticks have an application-defined unit. Events execute by due time, then registration order. Zero-delay events run during the next advance, including an advance already in progress, rather than inside `schedule()`.

Each advance defaults to a budget of 10000 actions. On `EventLimitReached`, remaining events stay queued and time remains at the last executed event. Calculate the remaining interval before continuing; do not blindly repeat the original delta. Event identifiers are cancellation tokens, not reusable sequence positions.

Callbacks may throw; exceptions propagate without rolling back register changes. A dequeued event is not retried. Captured objects must outlive event execution or cancellation. Access is single-threaded unless synchronized externally. A write result describes the write itself, not all later callback state changes; inspect status or fault registers for device-level results.

Nested `Device::write()` calls from a handler return `Busy` without mutation or logging. Use internal `update()` for model state. Handlers may replace or remove themselves safely; mutable handler state persists between writes unless the handler is replaced. Reentrancy guards are released even when a callback throws.

Reset cancels pending actions, restores registers and clears logs, while retaining handlers and current time. Event identifiers are not reused. Nested advancement returns `Busy`; time overflow is rejected before execution. Device and Scheduler objects cannot be copied or moved, avoiding stale captured references and copied in-progress state.
