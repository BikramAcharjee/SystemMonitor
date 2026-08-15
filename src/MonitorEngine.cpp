#define NOMINMAX

#include "MonitorEngine.h"
#include "TimeUtils.h"

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
    collectStaticInformation();
}

SystemSnapshot MonitorEngine::createSnapshot()
{
    SystemSnapshot snapshot;

    snapshot.timestamp = getCurrentTimestamp();

    snapshot.cpu = cpu;
    snapshot.cpuUsagePercent = cpuUsage.getUsage();

    snapshot.memory = getMemoryInfo();

    snapshot.gpu = getGpuInfo();

    snapshot.disks = getDiskInfo();

    snapshot.network = networkTraffic.sample();

    snapshot.motherboard = motherboard;

    snapshot.windows = windows;

    return snapshot;
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

void MonitorEngine::display(const SystemSnapshot& snapshot)
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

    double cpuUsageValue = snapshot.cpuUsagePercent;


    std::cout
        << "\n========== CPU ==========\n";

    std::cout
        << "Name              : "
        << snapshot.cpu.name
        << '\n';

    std::cout
        << "Usage             : "
        << std::fixed
        << std::setprecision(2)
        << cpuUsageValue
        << " %\n";

    std::cout
        << "Physical Cores    : "
        << snapshot.cpu.physicalCores
        << '\n';

    std::cout
        << "Logical Processors: "
        << snapshot.cpu.logicalProcessors
        << '\n';


    // --------------------------------------------------------
    // Memory
    // --------------------------------------------------------


    std::cout
        << "\n========== MEMORY ==========\n";

    std::cout
        << "Total RAM         : "
        << std::setprecision(2)
        << snapshot.memory.totalGB()
        << " GB\n";

    std::cout
        << "Used RAM          : "
        << snapshot.memory.usedGB()
        << " GB\n";

    std::cout
        << "Available RAM     : "
        << snapshot.memory.availableGB()
        << " GB\n";

    std::cout
        << "RAM Usage         : "
        << snapshot.memory.usagePercent()
        << " %\n";

    // --------------------------------------------------------
    // GPU
    // --------------------------------------------------------


    std::cout
        << "\n========== GPU ==========\n";


    if (!snapshot.gpu.available)
    {
        std::cout
            << "NVIDIA GPU monitoring unavailable.\n";
    }
    else
    {
        std::cout
            << "GPU               : "
            << snapshot.gpu.name
            << '\n';

        std::cout
            << "Usage             : "
            << snapshot.gpu.utilizationPercent
            << " %\n";

        std::cout
            << "Temperature       : "
            << snapshot.gpu.temperature
            << " C\n";

        std::cout
            << "VRAM Total        : "
            << std::fixed
            << std::setprecision(2)
            << snapshot.gpu.memoryTotalGB()
            << " GB\n";

        std::cout
            << "VRAM Used         : "
            << snapshot.gpu.memoryUsedGB()
            << " GB\n";

        std::cout
            << "VRAM Free         : "
            << snapshot.gpu.memoryFreeGB()
            << " GB\n";

        std::cout
            << "Core Clock        : "
            << snapshot.gpu.coreClockMHz
            << " MHz\n";

        std::cout
            << "Memory Clock      : "
            << snapshot.gpu.memoryClockMHz
            << " MHz\n";
    }


    // --------------------------------------------------------
    // Storage
    // --------------------------------------------------------


    std::cout
        << "\n========== STORAGE ==========\n";


    for (const auto& disk : snapshot.disks)
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


    std::cout
        << "\n========== NETWORK ==========\n";


    if (snapshot.network.empty())
    {
        std::cout
            << "No active physical network adapter.\n";
    }
    else
    {
        for (const auto& adapter : snapshot.network)
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
        << snapshot.motherboard.manufacturer
        << '\n';

    std::cout
        << "Model             : "
        << snapshot.motherboard.model
        << '\n';

    std::cout
        << "Serial Number     : "
        << snapshot.motherboard.serialNumber
        << '\n';


    // --------------------------------------------------------
    // Windows
    // --------------------------------------------------------

    std::cout
        << "\n========== WINDOWS ==========\n";

    std::cout
        << "Name              : "
        << snapshot.windows.name
        << '\n';

    std::cout
        << "Version           : "
        << snapshot.windows.version
        << '\n';

    std::cout
        << "Build             : "
        << snapshot.windows.buildNumber
        << '\n';

    std::cout
        << "Architecture      : "
        << snapshot.windows.architecture
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

        SystemSnapshot snapshot =
            createSnapshot();

        display(snapshot);
    }
}