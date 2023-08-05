# 检查记录

本次交付按约定未安装 SDK、Qt 或其他依赖，未配置 CMake、未编译、未运行测试。下面列的是已编写的测试范围，不表示运行通过。

| 测试源码 | 覆盖内容 |
|---|---|
| `definitions.cpp` | 位宽、复位值、无效策略和掩码 |
| `bank.cpp` | 查找、重复地址、内部更新、复位和记录容量 |
| `policies.cpp` | 只读、只写、写 1 清零、读后清零、混合位 |
| `scheduler.cpp` | 顺序、取消、零延迟、时间溢出、预算及异常 |
| `device.cpp` | 写入行为、状态迁移、复位及回调生命周期 |

使用者可按 [构建说明](building.md) 编译和运行测试。集成前还应使用自己的寄存器表、协议适配层和编译器验证。库使用动态容器和 `std::function`，不提供无堆分配或硬实时保证。

源码文本检查包括本地头文件引用、CMake 源文件清单、文档链接和代码围栏；另检查提交树内容、差异空白问题和沿用的作者/提交者日期。它们不能替代编译器检查、运行测试或跨平台验证。

## English

No SDK, Qt or dependencies were installed for this delivery. CMake was not configured, the source was not compiled, and tests were not run. The table lists authored test coverage, not passing execution results.

The sources cover definitions, register access policies, logging, virtual scheduling and device callbacks. Follow the [build instructions](building.md#english) to run them. Validate your own register map, transport adapter and compiler settings before integration. The implementation uses dynamic containers and `std::function`; it does not promise allocation-free operation or hard real-time behavior.

Text checks cover local includes, CMake source inventories, documentation links and code fences. Git checks cover tree contents, whitespace and the retained author/committer date schedule. These checks do not replace compilation, runtime tests or cross-platform validation.
