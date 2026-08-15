#include "system_monitor/models/MemoryInfo.h"

namespace
{
    constexpr double BYTES_PER_GB =
        1024.0 * 1024.0 * 1024.0;
}

double MemoryInfo::usagePercent() const
{
    if (totalBytes == 0)
    {
        return 0.0;
    }

    return
        (static_cast<double>(usedBytes) /
            static_cast<double>(totalBytes))
        * 100.0;
}

double MemoryInfo::totalGB() const
{
    return static_cast<double>(totalBytes)
        / BYTES_PER_GB;
}

double MemoryInfo::usedGB() const
{
    return static_cast<double>(usedBytes)
        / BYTES_PER_GB;
}

double MemoryInfo::availableGB() const
{
    return static_cast<double>(availableBytes)
        / BYTES_PER_GB;
}