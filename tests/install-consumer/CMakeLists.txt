cmake_minimum_required(VERSION 3.16)
project(RegisterDeviceMockConsumer LANGUAGES CXX)

find_package(RegisterDeviceMock 0.1 CONFIG REQUIRED)
add_executable(rdm_consumer main.cpp)
target_link_libraries(rdm_consumer PRIVATE RDM::Core)
