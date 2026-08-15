#pragma once

#include "WmiClient.h"

#include "system_monitor/models/CpuInfo.h"
#include "system_monitor/collectors/cpu/CpuCollector.h"

#include "system_monitor/models/MemoryInfo.h"
#include "system_monitor/collectors/memory/MemoryCollector.h"
#include "system_monitor/models/DiskInfo.h"
#include "system_monitor/collectors/disk/DiskCollector.h"
#include "system_monitor/models/NetworkTraffic.h"
#include "system_monitor/collectors/network/NetworkAdapterCollector.h"
#include "system_monitor/collectors/network/NetworkTrafficCollector.h"
#include "MotherboardInfo.h"
#include "WindowsInfo.h"
#include "CpuUsage.h"
#include "system_monitor/models/GpuInfo.h"
#include "system_monitor/collectors/gpu/GpuCollector.h"
#include "SystemSnapshot.h"

class MonitorEngine
{
public:

    explicit MonitorEngine(WmiClient& wmi);

    void run();

    SystemSnapshot createSnapshot();

private:

    WmiClient& wmi;

    // Collectors
    CpuCollector cpuCollector;
    MemoryCollector memoryCollector;
    GpuCollector gpuCollector;
    DiskCollector diskCollector;


    // Collected data
    CpuInfo cpu;
    MemoryInfo memory;
    GpuInfo gpu;

    MotherboardInfo motherboard;

    WindowsInfo windows;

    CpuUsage cpuUsage;

    NetworkAdapterCollector networkAdapterCollector;
    NetworkTrafficCollector networkTrafficCollector;

    bool running = true;

    void collectStaticInformation();

    void display(const SystemSnapshot& snapshot);
};