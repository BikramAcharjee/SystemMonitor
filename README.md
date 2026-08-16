# SystemMonitor

A modular, high-performance C++ system monitoring tool designed specifically for Windows. It exposes a JSON-based HTTP REST API to retrieve comprehensive system, hardware, and operating system details by querying low-level APIs and WMI.

---

## 1. Project Details

`SystemMonitor` is a background HTTP service developed in C++20. It runs a web server on port `8080`, providing real-time hardware diagnostics and performance metrics formatted as structured JSON.

### Key Features

- **REST HTTP API:** Exposes endpoints to query the entire system status or individual components (CPU, Memory, GPU, Storage, Network).
- **CPU Details:** Fetches processor model, manufacturer, physical core count, logical thread count via WMI (`Win32_Processor`), and calculates real-time CPU utilization percentage.
- **Memory Status:** Retrieves total physical RAM, available RAM, used RAM (in bytes and GB), and active usage percentage using native Win32 APIs (`GlobalMemoryStatusEx`).
- **GPU Information:** Dynamically interfaces with NVIDIA Management Library (`nvml.dll`) to capture GPU name, temperature, core/memory clocks, utilization, and VRAM utilization.
- **Storage Metrics:** Scans and monitors all fixed drives (`A:` to `Z:`), extracting total capacity, used space, free space, and usage percentages via native Win32 APIs (`GetDiskFreeSpaceExA`).
- **Network Traffic & Adapters:** Queries all active network interfaces to display friendly names, connection status, IPv4/IPv6 addresses, MAC addresses, and calculates real-time download/upload speeds in MB/s using the IP Helper API (`GetIfTable2`, `GetAdaptersAddresses`).
- **Motherboard Specifications:** Extracts manufacturer, model/product, and serial number via WMI (`Win32_BaseBoard`).
- **Operating System Info:** Gathers Windows name (caption), version string, build number, and target architecture via WMI (`Win32_OperatingSystem`).

### Technical Stack & Dependencies

- **Language:** C++20 (requires `/std:c++20` or equivalent flag)
- **API Frameworks:**
  - **WMI (Windows Management Instrumentation):** Uses COM (Component Object Model) Interfaces (`IWbemServices`, `IWbemLocator`) to query WQL.
  - **Win32 API:** Memory status (`GlobalMemoryStatusEx`), storage queries (`GetLogicalDrives`, `GetDiskFreeSpaceExA`), and socket tools.
  - **NVML (NVIDIA Management Library):** Dynamically loads `nvml.dll` to query NVIDIA GPU performance metrics.
  - **IP Helper API:** Gathers active network configuration details and performance statistics.
- **Third-Party Libraries (managed via vcpkg):**
  - [**`nlohmann-json`**](https://github.com/nlohmann/json): High-performance JSON serialization.
  - [**`cpp-httplib`**](https://github.com/yhirose/cpp-httplib): A lightweight, header-only C++ HTTP/HTTPS server.
- **Build System:** CMake (version 3.20 or higher)
- **Target Platforms:** Windows 10/11 / Windows Server (64-bit recommended)

---

## 2. Structure Information

Below is the updated directory structure of the `SystemMonitor` repository:

```text
SystemMonitor/
├── .gitignore
├── CMakeLists.txt
├── CMakePresets.json
├── CMakeSettings.json
├── LICENSE
├── README.md
├── vcpkg.json
├── include/
│   ├── MonitorEngine.h
│   ├── SystemSnapshot.h
│   ├── SystemSnapshotJson.h
│   ├── WmiClient.h
│   └── system_monitor/
│       ├── api/
│       │   └── ApiResponse.h
│       ├── collectors/
│       │   ├── cpu/
│       │   │   ├── CpuCollector.h
│       │   │   └── CpuUsage.h
│       │   ├── disk/
│       │   │   └── DiskCollector.h
│       │   ├── gpu/
│       │   │   └── GpuCollector.h
│       │   ├── memory/
│       │   │   └── MemoryCollector.h
│       │   ├── motherboard/
│       │   │   └── MotherboardCollector.h
│       │   ├── network/
│       │   │   ├── NetworkAdapterCollector.h
│       │   │   └── NetworkTrafficCollector.h
│       │   └── windows/
│       │       └── WindowsCollector.h
│       ├── models/
│       │   ├── CpuInfo.h
│       │   ├── DiskInfo.h
│       │   ├── GpuInfo.h
│       │   ├── MemoryInfo.h
│       │   ├── MotherboardInfo.h
│       │   ├── NetworkAdapterInfo.h
│       │   ├── NetworkTraffic.h
│       │   └── WindowsInfo.h
│       └── utils/
│           └── TimeUtils.h
└── src/
    ├── main.cpp
    ├── MonitorEngine.cpp
    ├── SystemSnapshotJson.cpp
    ├── WmiClient.cpp
    ├── collectors/
    │   ├── cpu/
    │   │   ├── CpuCollector.cpp
    │   │   └── CpuUsage.cpp
    │   ├── disk/
    │   │   └── DiskCollector.cpp
    │   ├── gpu/
    │   │   └── GpuCollector.cpp
    │   ├── memory/
    │   │   └── MemoryCollector.cpp
    │   ├── motherboard/
    │   │   └── MotherboardCollector.cpp
    │   ├── network/
    │   │   ├── NetworkAdapterCollector.cpp
    │   │   └── NetworkTrafficCollector.cpp
    │   └── windows/
    │       └── WindowsCollector.cpp
    ├── models/
    │   ├── DiskInfo.cpp
    │   ├── GpuInfo.cpp
    │   └── MemoryInfo.cpp
    └── utils/
        └── TimeUtils.cpp
```

---

## 3. File/Folder Description

### Directories

- [**`include/`**](file:///F:/Projects/C++/SystemMonitor/include): Contains class declarations, public headers, and API layout schemas.
- [**`src/`**](file:///F:/Projects/C++/SystemMonitor/src): Core implementation code for collectors, JSON converters, HTTP routing, and helper classes.

### Core Modules

- [**`main.cpp`**](file:///F:/Projects/C++/SystemMonitor/src/main.cpp): Launches the `cpp-httplib` server on port `8080`, configures the REST endpoints, and delegates requests to the monitoring engine.
- [**`MonitorEngine`**](file:///F:/Projects/C++/SystemMonitor/include/MonitorEngine.h): Acts as the main orchestrator, initializing individual collectors and assembling the final `SystemSnapshot`.
- [**`WmiClient`**](file:///F:/Projects/C++/SystemMonitor/include/WmiClient.h): Initializes COM security, connects to WMI (`ROOT\CIMV2`), and runs WQL queries safely.
- [**`SystemSnapshotJson`**](file:///F:/Projects/C++/SystemMonitor/include/SystemSnapshotJson.h): Translates the C++ models into standardized JSON payloads using `nlohmann/json`.

### Collectors (`src/collectors/` & `include/system_monitor/collectors/`)

- **CPU (`cpu/`):** Computes system processor specifications (`CpuCollector`) and tracks real-time usage (`CpuUsage`).
- **Disk (`disk/`):** Evaluates local hard drives and SSDs capacity/usage using Windows volume APIs.
- **GPU (`gpu/`):** Queries NVIDIA metrics via runtime nvml DLL library mappings.
- **Memory (`memory/`):** Inspects RAM physical limits and active load metrics.
- **Network (`network/`):** Monitors network configurations (`NetworkAdapterCollector`) and calculates speeds (`NetworkTrafficCollector`).
- **Motherboard (`motherboard/`):** Queries motherboard manufacturer, serial number, and product model.
- **Windows (`windows/`):** Retrives OS details including caption, architecture, and current build.

---

## 4. REST API Endpoints

Once the application is running, the following endpoints are available at `http://localhost:8080`:

| Endpoint              | Method | Description                                                                    |
| --------------------- | ------ | ------------------------------------------------------------------------------ |
| `/api/health`         | `GET`  | Service status checks. Returns `{"status": "UP", "service": "SystemMonitor"}`. |
| `/api/system`         | `GET`  | Returns a complete system snapshot containing all hardware and status metrics. |
| `/api/system/cpu`     | `GET`  | Detailed CPU metrics, core counts, and current usage.                          |
| `/api/system/memory`  | `GET`  | RAM details (total, used, available, and percentage).                          |
| `/api/system/gpu`     | `GET`  | Graphics details (utilization, VRAM, clock speeds, and temperature).           |
| `/api/system/storage` | `GET`  | Array of fixed disk drive metrics (used space, total size, usage percentage).  |
| `/api/system/network` | `GET`  | Network adapters and real-time upload/download speeds.                         |

---

## 5. How to Run Locally

### Prerequisites

- **Operating System:** Windows 10 or 11
- **Compiler:** Microsoft Visual C++ Compiler (MSVC) supporting C++20 (Visual Studio 2022 or Build Tools)
- **Package Manager:** `vcpkg` (for installing `nlohmann-json` and `cpp-httplib`)
- **Build Tool:** CMake (version 3.20 or newer)

---

### Option A: Using Visual Studio (Recommended)

Visual Studio provides a built-in integration for CMake and vcpkg.

1. **Open the Project:**
   - Launch **Visual Studio 2022**.
   - Click on **Open a local folder**.
   - Select the root directory containing this project (`SystemMonitor`).
2. **Configure with vcpkg:**
   - Visual Studio automatically detects `vcpkg.json` and installs dependencies.
   - If utilizing a custom vcpkg location, set up your configuration in [`CMakeSettings.json`](file:///F:/Projects/C++/SystemMonitor/CMakeSettings.json) or [`CMakePresets.json`](file:///F:/Projects/C++/SystemMonitor/CMakePresets.json) by passing the `-DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake` parameter.
3. **Run the Project:**
   - Set the startup target dropdown to **`SystemMonitor.exe`**.
   - Press **F5** (Debug mode) or **Ctrl + F5** (Run without debugging).
   - The REST service will spin up, ready to accept queries.

---

### Option B: Using Command Line (PowerShell / Command Prompt)

1. **Open Developer Command Prompt / PowerShell:**
   - Ensure the compiler tools and CMake are in your environment path.
2. **Navigate to the Project Root:**
   ```powershell
   cd "F:\Projects\C++\SystemMonitor"
   ```
3. **Configure with vcpkg integration:**
   ```powershell
   cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake"
   ```
4. **Build the Target:**
   ```powershell
   cmake --build build --config Release
   ```
5. **Run the Service:**
   ```powershell
   .\build\Release\SystemMonitor.exe
   ```
