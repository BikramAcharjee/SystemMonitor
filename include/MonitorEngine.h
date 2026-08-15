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
#include "SystemSnapshot.h"


class MonitorEngine
{
public:

    explicit MonitorEngine(WmiClient& wmi);

    void run();

    SystemSnapshot createSnapshot();


private:

    WmiClient& wmi;

    CpuInfo cpu;

    GpuInfo gpu;

    MotherboardInfo motherboard;

    WindowsInfo windows;

    CpuUsage cpuUsage;

    NetworkTrafficMonitor networkTraffic;

    bool running = true;

    void collectStaticInformation();

    void display(const SystemSnapshot& snapshot);
};