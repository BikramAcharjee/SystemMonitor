#pragma once

#include <string>
#include <vector>

#include "CpuInfo.h"
#include "MemoryInfo.h"
#include "DiskInfo.h"
#include "NetworkTraffic.h"
#include "MotherboardInfo.h"
#include "WindowsInfo.h"
#include "GpuInfo.h"

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