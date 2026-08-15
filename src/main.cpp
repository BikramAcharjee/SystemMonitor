#include <iostream>

#include "WmiClient.h"
#include "CpuInfo.h"
#include "MemoryInfo.h"
#include "MotherboardInfo.h"
#include "WindowsInfo.h"

int main()
{
    std::cout << "========================================\n";
    std::cout << "       C++ SYSTEM MONITOR - PHASE 1     \n";
    std::cout << "========================================\n";


    // --------------------------------------------------------
    // Initialize WMI
    // --------------------------------------------------------

    WmiClient wmi;

    if (!wmi.initialize())
    {
        std::cerr << "\nFailed to initialize WMI.\n";
        return 1;
    }


    // --------------------------------------------------------
    // Collect information
    // --------------------------------------------------------

    CpuInfo cpu = getCpuInfo(wmi);

    MemoryInfo memory = getMemoryInfo();

    MotherboardInfo motherboard =
        getMotherboardInfo(wmi);

    WindowsInfo windows =
        getWindowsInfo(wmi);


    // --------------------------------------------------------
    // Display CPU
    // --------------------------------------------------------

    std::cout << "\n========== CPU ==========\n";

    std::cout
        << "Name              : "
        << cpu.name
        << '\n';

    std::cout
        << "Manufacturer      : "
        << cpu.manufacturer
        << '\n';

    std::cout
        << "Physical Cores    : "
        << cpu.physicalCores
        << '\n';

    std::cout
        << "Logical Processors: "
        << cpu.logicalProcessors
        << '\n';


    // --------------------------------------------------------
    // Display memory
    // --------------------------------------------------------

    std::cout << "\n========== MEMORY ===========\n";

    std::cout
        << "Total RAM         : "
        << memory.totalGB()
        << " GB\n";


    // --------------------------------------------------------
    // Display motherboard
    // --------------------------------------------------------

    std::cout << "\n========== MOTHERBOARD ==========\n";

    std::cout
        << "Manufacturer      : "
        << motherboard.manufacturer
        << '\n';

    std::cout
        << "Model             : "
        << motherboard.model
        << '\n';

    std::cout
        << "Serial Number     : "
        << motherboard.serialNumber
        << '\n';


    // --------------------------------------------------------
    // Display Windows
    // --------------------------------------------------------

    std::cout << "\n========== WINDOWS ==========\n";

    std::cout
        << "Name              : "
        << windows.name
        << '\n';

    std::cout
        << "Version           : "
        << windows.version
        << '\n';

    std::cout
        << "Build             : "
        << windows.buildNumber
        << '\n';

    std::cout
        << "Architecture      : "
        << windows.architecture
        << '\n';


    std::cout << "\n========================================\n";
    std::cout << "System information collected successfully.\n";

    return 0;
}