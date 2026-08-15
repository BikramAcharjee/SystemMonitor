#pragma once

#include "WmiClient.h"
#include "CpuInfo.h"
#include "MemoryInfo.h"
#include "DiskInfo.h"
#include "NetworkTraffic.h"
#include "MotherboardInfo.h"
#include "WindowsInfo.h"
#include "CpuUsage.h"
#include "GpuInfo.h"


class MonitorEngine
{
public:

    explicit MonitorEngine(WmiClient& wmi);

    void run();


private:

    WmiClient& wmi;

    // --------------------------------------------------------
    // Static information
    // --------------------------------------------------------

    CpuInfo cpu;

    // --------------------------------------------------------
    // GPU
    // --------------------------------------------------------

    GpuInfo gpu;

    MotherboardInfo motherboard;

    WindowsInfo windows;


    // --------------------------------------------------------
    // Dynamic monitoring
    // --------------------------------------------------------

    CpuUsage cpuUsage;

    NetworkTrafficMonitor networkTraffic;

    // --------------------------------------------------------
    // Control
    // --------------------------------------------------------

    bool running = true;


    // --------------------------------------------------------
    // Internal functions
    // --------------------------------------------------------

    void collectStaticInformation();

    void display();
};