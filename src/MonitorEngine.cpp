#define NOMINMAX

#include "MonitorEngine.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <windows.h>


MonitorEngine::MonitorEngine(WmiClient& wmi)
    : wmi(wmi)
{
}


// ============================================================
// Collect information that doesn't change frequently
// ============================================================

void MonitorEngine::collectStaticInformation()
{
    cpu = getCpuInfo(wmi);

    motherboard =
        getMotherboardInfo(wmi);

    windows =
        getWindowsInfo(wmi);
}


// ============================================================
// Display current monitoring information
// ============================================================

void MonitorEngine::display()
{
    // --------------------------------------------------------
    // Clear console
    // --------------------------------------------------------

    system("cls");


    std::cout
        << "========================================\n";

    std::cout
        << "        C++ SYSTEM MONITOR\n";

    std::cout
        << "========================================\n";


    // --------------------------------------------------------
    // CPU
    // --------------------------------------------------------

    double cpuUsageValue =
        cpuUsage.getUsage();


    std::cout
        << "\n========== CPU ==========\n";

    std::cout
        << "Name              : "
        << cpu.name
        << '\n';

    std::cout
        << "Usage             : "
        << std::fixed
        << std::setprecision(2)
        << cpuUsageValue
        << " %\n";

    std::cout
        << "Physical Cores    : "
        << cpu.physicalCores
        << '\n';

    std::cout
        << "Logical Processors: "
        << cpu.logicalProcessors
        << '\n';


    // --------------------------------------------------------
    // Memory
    // --------------------------------------------------------

    MemoryInfo memory =
        getMemoryInfo();


    std::cout
        << "\n========== MEMORY ==========\n";

    std::cout
        << "Total RAM         : "
        << std::setprecision(2)
        << memory.totalGB()
        << " GB\n";

    std::cout
        << "Used RAM          : "
        << memory.usedGB()
        << " GB\n";

    std::cout
        << "Available RAM     : "
        << memory.availableGB()
        << " GB\n";

    std::cout
        << "RAM Usage         : "
        << memory.usagePercent()
        << " %\n";

    // --------------------------------------------------------
    // GPU
    // --------------------------------------------------------

    gpu = getGpuInfo();


    std::cout
        << "\n========== GPU ==========\n";


    if (!gpu.available)
    {
        std::cout
            << "NVIDIA GPU monitoring unavailable.\n";
    }
    else
    {
        std::cout
            << "GPU               : "
            << gpu.name
            << '\n';

        std::cout
            << "Usage             : "
            << gpu.utilizationPercent
            << " %\n";

        std::cout
            << "Temperature       : "
            << gpu.temperature
            << " C\n";

        std::cout
            << "VRAM Total        : "
            << std::fixed
            << std::setprecision(2)
            << gpu.memoryTotalGB()
            << " GB\n";

        std::cout
            << "VRAM Used         : "
            << gpu.memoryUsedGB()
            << " GB\n";

        std::cout
            << "VRAM Free         : "
            << gpu.memoryFreeGB()
            << " GB\n";

        std::cout
            << "Core Clock        : "
            << gpu.coreClockMHz
            << " MHz\n";

        std::cout
            << "Memory Clock      : "
            << gpu.memoryClockMHz
            << " MHz\n";
    }


    // --------------------------------------------------------
    // Storage
    // --------------------------------------------------------

    std::vector<DiskInfo> disks =
        getDiskInfo();


    std::cout
        << "\n========== STORAGE ==========\n";


    for (const auto& disk : disks)
    {
        std::cout
            << disk.drive
            << "  "
            << std::fixed
            << std::setprecision(1)
            << disk.usedGB()
            << " / "
            << disk.totalGB()
            << " GB"
            << "  ("
            << disk.usagePercent()
            << "%)\n";
    }


    // --------------------------------------------------------
    // Network
    // --------------------------------------------------------

    std::vector<NetworkTraffic>
        network =
        networkTraffic.sample();


    std::cout
        << "\n========== NETWORK ==========\n";


    if (network.empty())
    {
        std::cout
            << "No active physical network adapter.\n";
    }
    else
    {
        for (const auto& adapter : network)
        {
            std::cout
                << adapter.adapterName
                << '\n';

            std::cout
                << "  Download : "
                << std::fixed
                << std::setprecision(3)
                << adapter.downloadMBps
                << " MB/s\n";

            std::cout
                << "  Upload   : "
                << adapter.uploadMBps
                << " MB/s\n";
        }
    }

    // --------------------------------------------------------
    // Motherboard
    // --------------------------------------------------------

    std::cout
        << "\n========== MOTHERBOARD ==========\n";

    std::cout
        << "Manufacturer      : "
        << motherboard.manufacturer
        << '\n';

    std::cout
        << "Model             : "
        << motherboard.model
        << '\n';


    // --------------------------------------------------------
    // Windows
    // --------------------------------------------------------

    std::cout
        << "\n========== WINDOWS ==========\n";

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


    // --------------------------------------------------------
    // Footer
    // --------------------------------------------------------

    std::cout
        << "\n========================================\n";

    std::cout
        << "Refreshing every 1 second...\n";

    std::cout
        << "Press Ctrl+C to exit.\n";
}


// ============================================================
// Main monitoring loop
// ============================================================

void MonitorEngine::run()
{
    collectStaticInformation();


    // Prime CPU usage measurement
    cpuUsage.getUsage();


    while (running)
    {
        // ----------------------------------------------------
        // Wait before first refresh
        // ----------------------------------------------------

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );


        // ----------------------------------------------------
        // Display latest information
        // ----------------------------------------------------

        display();
    }
}