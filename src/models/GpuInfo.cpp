#include "system_monitor/models/GpuInfo.h"

namespace
{
    constexpr double BYTES_PER_GB =
        1024.0 * 1024.0 * 1024.0;
}

double GpuInfo::memoryTotalGB() const
{
    return
        static_cast<double>(memoryTotal)
        / BYTES_PER_GB;
}

double GpuInfo::memoryUsedGB() const
{
    return
        static_cast<double>(memoryUsed)
        / BYTES_PER_GB;
}

double GpuInfo::memoryFreeGB() const
{
    return
        static_cast<double>(memoryFree)
        / BYTES_PER_GB;
}