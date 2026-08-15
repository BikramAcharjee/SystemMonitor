cmake_minimum_required(VERSION 3.20)

project(SystemMonitor LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SystemMonitor
    src/main.cpp
    src/WmiClient.cpp
    src/CpuInfo.cpp
    src/MemoryInfo.cpp
    src/MotherboardInfo.cpp
    src/WindowsInfo.cpp
)

target_include_directories(SystemMonitor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)