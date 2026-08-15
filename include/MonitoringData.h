#pragma once

#include <string>

struct MonitoringData
{
    // CPU
    std::string cpuName;
    double cpuUsagePercent = 0.0;
    int physicalCores = 0;
    int logicalProcessors = 0;

    // RAM
    double ramTotalGB = 0.0;
    double ramUsedGB = 0.0;
    double ramAvailableGB = 0.0;
    double ramUsagePercent = 0.0;

    // GPU
    bool gpuAvailable = false;
    std::string gpuName;

    unsigned int gpuUsagePercent = 0;
    unsigned int gpuTemperature = 0;

    double vramTotalGB = 0.0;
    double vramUsedGB = 0.0;
    double vramFreeGB = 0.0;

    unsigned int gpuCoreClockMHz = 0;
    unsigned int gpuMemoryClockMHz = 0;

    // Windows
    std::string windowsName;
    std::string windowsVersion;
    std::string windowsBuild;
    std::string windowsArchitecture;
};