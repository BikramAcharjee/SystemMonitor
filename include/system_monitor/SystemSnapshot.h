#pragma once

#include <string>
#include <vector>

#include "system_monitor/models/CpuInfo.h"
#include "system_monitor/models/MemoryInfo.h"
#include "system_monitor/models/DiskInfo.h"
#include "system_monitor/models/NetworkTraffic.h"
#include "system_monitor/models/MotherboardInfo.h"
#include "system_monitor/models/WindowsInfo.h"
#include "system_monitor/models/GpuInfo.h"

struct SystemSnapshot
{

    std::string timestamp;

    CpuInfo cpu;
    MotherboardInfo motherboard;
    WindowsInfo windows;

    double cpuUsagePercent = 0.0;

    MemoryInfo memory;

    GpuInfo gpu;

    std::vector<DiskInfo> disks;

    std::vector<NetworkTraffic> network;
};