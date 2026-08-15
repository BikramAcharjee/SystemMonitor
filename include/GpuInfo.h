#pragma once

#include <string>

struct GpuInfo
{
    bool available = false;

    std::string name;

    unsigned int utilizationPercent = 0;

    unsigned long long memoryTotal = 0;
    unsigned long long memoryUsed = 0;
    unsigned long long memoryFree = 0;

    unsigned int temperature = 0;

    unsigned int coreClockMHz = 0;
    unsigned int memoryClockMHz = 0;

    double memoryTotalGB() const;
    double memoryUsedGB() const;
    double memoryFreeGB() const;
};

GpuInfo getGpuInfo();