cmake_minimum_required(VERSION 3.16)
project(RegisterDeviceMock VERSION 0.1.0 LANGUAGES CXX)

option(RDM_BUILD_TESTS "Build RegisterDeviceMock tests" OFF)
option(RDM_BUILD_EXAMPLES "Build RegisterDeviceMock examples" OFF)

# Static by design: consumers do not need platform-specific DLL export macros.
add_library(RegisterDeviceMock STATIC
    src/AccessLog.cpp
    src/Device.cpp
    src/RegisterBank.cpp
    src/RegisterDefinition.cpp
    src/RegisterRead.cpp
    src/RegisterUpdate.cpp
    src/RegisterWrite.cpp
    src/Scheduler.cpp
)
add_library(RDM::Core ALIAS RegisterDeviceMock)
set_target_properties(RegisterDeviceMock PROPERTIES EXPORT_NAME Core)
target_compile_features(RegisterDeviceMock PUBLIC cxx_std_17)
target_include_directories(RegisterDeviceMock PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
install(TARGETS RegisterDeviceMock EXPORT RegisterDeviceMockTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT RegisterDeviceMockTargets NAMESPACE RDM::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RegisterDeviceMock
)
configure_package_config_file(cmake/RegisterDeviceMockConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/RegisterDeviceMockConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RegisterDeviceMock
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/RegisterDeviceMockConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/RegisterDeviceMockConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/RegisterDeviceMockConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RegisterDeviceMock
)
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATADIR}/RegisterDeviceMock)

if(RDM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
