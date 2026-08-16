#include "system_monitor/models/DiskInfo.h"

namespace
{
    constexpr double BYTES_PER_GB =
        1024.0 * 1024.0 * 1024.0;
}

double DiskInfo::totalGB() const
{
    return
        static_cast<double>(totalBytes)
        / BYTES_PER_GB;
}

double DiskInfo::freeGB() const
{
    return
        static_cast<double>(freeBytes)
        / BYTES_PER_GB;
}

double DiskInfo::usedGB() const
{
    return
        static_cast<double>(usedBytes)
        / BYTES_PER_GB;
}

double DiskInfo::usagePercent() const
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