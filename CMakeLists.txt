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
    src/WmiClient.cpp
    src/CpuInfo.cpp
    src/MemoryInfo.cpp
    src/MotherboardInfo.cpp
    src/WindowsInfo.cpp
    src/CpuUsage.cpp
    src/DiskInfo.cpp
    src/NetworkInfo.cpp
    src/NetworkTraffic.cpp
    src/MonitorEngine.cpp
    src/GpuInfo.cpp
    src/SystemSnapshotJson.cpp
    src/TimeUtils.cpp
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