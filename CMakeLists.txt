cmake_minimum_required(VERSION 3.20)

project(SystemMonitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)

add_executable(SystemMonitor
    src/main.cpp

    # Core
    src/MonitorEngine.cpp
    src/SystemSnapshotJson.cpp

    # Platform - Windows
    src/WmiClient.cpp
    src/TimeUtils.cpp

    # Models
    src/models/MemoryInfo.cpp
    src/models/GpuInfo.cpp
    src/models/DiskInfo.cpp

    # Collectors - CPU
    src/collectors/cpu/CpuCollector.cpp
    src/CpuUsage.cpp

    # Collectors - Memory
    src/collectors/memory/MemoryCollector.cpp

    # Collectors - Motherboard
    src/MotherboardInfo.cpp

    # Collectors - Windows
    src/WindowsInfo.cpp

    # Collectors - Disk
    src/collectors/disk/DiskCollector.cpp

    # Collectors - Network
    src/collectors/network/NetworkAdapterCollector.cpp
    src/collectors/network/NetworkTrafficCollector.cpp

    # Collectors - GPU
    src/collectors/gpu/GpuCollector.cpp
)

target_include_directories(SystemMonitor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SystemMonitor
    PRIVATE
        wbemuuid
        iphlpapi
        nlohmann_json::nlohmann_json
        httplib::httplib
)