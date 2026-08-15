# SystemMonitor

A lightweight, high-performance C++ system monitoring tool designed specifically for Windows. It retrieves comprehensive hardware and operating system details by interfacing with Windows Management Instrumentation (WMI) and the native Win32 APIs.

---

## 1. Project Details

`SystemMonitor` is a console-based utility developed in C++20. It serves as a diagnostic tool that queries low-level system specifications and prints them in a clean, readable dashboard format.

### Key Features

- **CPU Details:** Fetches processor name, manufacturer, physical core count, and logical processor (thread) count via WMI (`Win32_Processor`).
- **Memory Status:** Retrieves total physical RAM size in gigabytes (GB) using native Win32 APIs (`GlobalMemoryStatusEx`).
- **Motherboard Specifications:** Extracts board manufacturer, model (product), and serial number via WMI (`Win32_BaseBoard`).
- **Operating System Info:** Gathers Windows name (caption), version string, build number, and target architecture via WMI (`Win32_OperatingSystem`).

### Technical Stack & Dependencies

- **Language:** C++20 (requires `/std:c++20` or equivalent flag)
- **API Frameworks:**
  - **WMI (Windows Management Instrumentation):** Uses COM (Component Object Model) Interfaces (`IWbemServices`, `IWbemLocator`) to run WQL queries.
  - **Win32 API:** Directly calls memory and system information utilities.
- **Build System:** CMake (version 3.20 or higher)
- **Target Platforms:** Windows 10/11 / Windows Server (64-bit recommended)

---

## 2. Structure Information

Below is the directory structure of the `SystemMonitor` repository:

```text
SystemMonitor/
├── .gitignore
├── CMakeLists.txt
├── CMakeSettings.json
├── LICENSE
├── README.md
├── include/
│   ├── CpuInfo.h
│   ├── MemoryInfo.h
│   ├── MotherboardInfo.h
│   ├── WindowsInfo.h
│   └── WmiClient.h
└── src/
    ├── CpuInfo.cpp
    ├── MemoryInfo.cpp
    ├── MotherboardInfo.cpp
    ├── WindowsInfo.cpp
    ├── WmiClient.cpp
    └── main.cpp
```

---

## 3. File/Folder Description

### Directories

- [**`include/`**](file:///f:/Projects/C++/SystemMonitor/include): Houses all C++ header files (`.h`) containing class declarations and data structures.
- [**`src/`**](file:///f:/Projects/C++/SystemMonitor/src): Contains the main source files (`.cpp`) implementing the system collection logic and application entry point.

### Header Files (`include/`)

- [**`WmiClient.h`**](file:///f:/Projects/C++/SystemMonitor/include/WmiClient.h): Defines the `WmiClient` class responsible for initializing COM security, establishing WMI locator connections, and executing WQL queries to query properties.
- [**`CpuInfo.h`**](file:///f:/Projects/C++/SystemMonitor/include/CpuInfo.h): Defines the `CpuInfo` structure and the `getCpuInfo()` helper signature.
- [**`MemoryInfo.h`**](file:///f:/Projects/C++/SystemMonitor/include/MemoryInfo.h): Defines the `MemoryInfo` structure which wraps memory size and exposes a converter to Gigabytes (`totalGB()`).
- [**`MotherboardInfo.h`**](file:///f:/Projects/C++/SystemMonitor/include/MotherboardInfo.h): Defines the `MotherboardInfo` structure for manufacturer, model, and serial details.
- [**`WindowsInfo.h`**](file:///f:/Projects/C++/SystemMonitor/include/WindowsInfo.h): Defines the `WindowsInfo` structure to represent OS details.

### Source Files (`src/`)

- [**`WmiClient.cpp`**](file:///f:/Projects/C++/SystemMonitor/src/WmiClient.cpp): Implements COM initialization, connection to `ROOT\CIMV2`, queries executing via `ExecQuery`, property parsing for multiple variant types (e.g. `VT_BSTR`, `VT_UI4`, `VT_I8`), and helper methods to convert wide strings to UTF-8 (`wideToUtf8`).
- [**`CpuInfo.cpp`**](file:///f:/Projects/C++/SystemMonitor/src/CpuInfo.cpp): Implements `getCpuInfo()` to query the `Win32_Processor` class for processor specifications.
- [**`MemoryInfo.cpp`**](file:///f:/Projects/C++/SystemMonitor/src/MemoryInfo.cpp): Implements `getMemoryInfo()` using `GlobalMemoryStatusEx` to query physical RAM.
- [**`MotherboardInfo.cpp`**](file:///f:/Projects/C++/SystemMonitor/src/MotherboardInfo.cpp): Implements `getMotherboardInfo()` to query `Win32_BaseBoard`.
- [**`WindowsInfo.cpp`**](file:///f:/Projects/C++/SystemMonitor/src/WindowsInfo.cpp): Implements `getWindowsInfo()` to query `Win32_OperatingSystem`.
- [**`main.cpp`**](file:///f:/Projects/C++/SystemMonitor/src/main.cpp): The main application entry point. Handles WMI client initialization, triggers individual information retrievers, prints the retrieved information to standard output in structured categories, and handles cleanup.

### Project Configuration

- [**`CMakeLists.txt`**](file:///f:/Projects/C++/SystemMonitor/CMakeLists.txt): Defines project compilation requirements, C++ standard (C++20), include paths, list of source files, and compilation options.
- [**`CMakeSettings.json`**](file:///f:/Projects/C++/SystemMonitor/CMakeSettings.json): Visual Studio-specific settings for configuring the CMake generator, build root, and build configurations (such as `x64-Debug`).

---

## 4. How to Run Locally

### Prerequisites

- **Operating System:** Windows (10 or higher recommended)
- **Compiler:** Microsoft Visual C++ Compiler (MSVC) supporting C++20 (VS 2022 or Build Tools)
- **Build Tool:** CMake (version 3.20 or newer)

---

### Option A: Using Visual Studio (Recommended)

Visual Studio provides a built-in, first-class experience for CMake projects.

1. **Open the Project:**
   - Launch **Visual Studio 2022**.
   - Click on **Open a local folder**.
   - Select the root directory containing this project (`SystemMonitor`).
2. **Configure CMake:**
   - Visual Studio will automatically detect `CMakeLists.txt` and generate build cache settings using the configuration specified in [`CMakeSettings.json`](file:///f:/Projects/C++/SystemMonitor/CMakeSettings.json).
3. **Run the Project:**
   - In the top toolbar, set the startup item target dropdown to **`SystemMonitor.exe (Install)`** or **`SystemMonitor.exe`**.
   - Press **F5** (Debug mode) or **Ctrl + F5** (Run without debugging).
   - A terminal console window will open displaying the hardware specifications.

---

### Option B: Using Command Line (PowerShell / Command Prompt)

You can build and run the application manually from the command line using standard CMake commands:

1. **Open Developer Command Prompt / PowerShell:**
   - Open the **Developer Command Prompt for VS 2022** or a PowerShell window with compiler tools available in the PATH.
2. **Navigate to the Project Root:**
   ```powershell
   cd "f:\Projects\C++\SystemMonitor"
   ```
3. **Configure the Build Directory:**
   ```powershell
   cmake -S . -B build -G "Visual Studio 18 2026" -A x64
   ```
4. **Build the Target:**
   ```powershell
   cmake --build build --config Release
   ```
5. **Run the Executable:**
   ```powershell
   .\build\Release\SystemMonitor.exe
   ```
