# 构建和集成

需要支持 C++17 的编译器和 CMake 3.16 或更高版本，不需要 Qt。生成的是静态库。

以下是供使用者执行的命令，本次交付没有执行构建和测试。

```sh
cmake -S . -B build -DRDM_BUILD_TESTS=ON -DRDM_BUILD_EXAMPLES=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
cmake --install build --config Release --prefix /absolute/path/rdm-install
```

从源码接入：

```cmake
add_subdirectory(path/to/RegisterDeviceMock)
target_link_libraries(your_app PRIVATE RDM::Core)
```

从安装目录接入：

```cmake
find_package(RegisterDeviceMock 0.1 CONFIG REQUIRED)
target_link_libraries(your_app PRIVATE RDM::Core)
```

配置你的应用时，使用 `-DCMAKE_PREFIX_PATH=/absolute/path/rdm-install` 指定安装前缀。测试和示例默认关闭，不会随库默认构建。

Windows 等多配置生成器使用上面的 `--config Release` 和 `-C Release`。静态库与应用需要使用兼容的编译器、标准库和运行库配置。

## English

Use a C++17 compiler and CMake 3.16 or newer. Qt is not required. The target builds a static library; tests and examples are opt-in.

The commands above configure examples and tests, build them, run CTest, and install the package to an explicit prefix. They are instructions for consumers, not a record of validation performed for this delivery.

For a source dependency, use `add_subdirectory()` and link `RDM::Core`. For an installed dependency, set `CMAKE_PREFIX_PATH` to the install prefix, call `find_package(RegisterDeviceMock 0.1 CONFIG REQUIRED)`, and link the same target. The exported target carries the C++17 requirement and include directory.

Use compatible compiler, standard library, architecture, and runtime settings for the library and application. With multi-configuration generators, select the matching build and test configuration using `--config` and `-C`.
