#pragma once

#include "system_monitor/WmiClient.h"

#include "system_monitor/models/CpuInfo.h"
#include "system_monitor/collectors/cpu/CpuCollector.h"

#include "system_monitor/collectors/memory/MemoryCollector.h"
#include "system_monitor/models/DiskInfo.h"
#include "system_monitor/collectors/disk/DiskCollector.h"
#include "system_monitor/models/NetworkTraffic.h"
#include "system_monitor/collectors/network/NetworkTrafficCollector.h"
#include "system_monitor/collectors/motherboard/MotherboardCollector.h"
#include "system_monitor/collectors/windows/WindowsCollector.h"
#include "system_monitor/collectors/cpu/CpuUsage.h"
#include "system_monitor/models/MotherboardInfo.h"
#include "system_monitor/models/WindowsInfo.h"
#include "system_monitor/collectors/gpu/GpuCollector.h"
#include "system_monitor/SystemSnapshot.h"

class MonitorEngine
{
public:

    explicit MonitorEngine(WmiClient& wmi);

    SystemSnapshot createSnapshot();

private:

    // Collectors
    CpuCollector cpuCollector;
    MemoryCollector memoryCollector;
    GpuCollector gpuCollector;
    DiskCollector diskCollector;
    MotherboardCollector motherboardCollector;
    WindowsCollector windowsCollector;


    // Collected data
    CpuInfo cpu;

    MotherboardInfo motherboard;

    WindowsInfo windows;

    CpuUsage cpuUsage;

    NetworkTrafficCollector networkTrafficCollector;

    void collectStaticInformation();
};