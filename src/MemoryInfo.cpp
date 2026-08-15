#include "MemoryInfo.h"

#include <windows.h>

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

MemoryInfo getMemoryInfo()
{
    MemoryInfo memory;

    MEMORYSTATUSEX status{};

    status.dwLength = sizeof(status);

    if (!GlobalMemoryStatusEx(&status))
    {
        return memory;
    }

    memory.totalBytes =
        status.ullTotalPhys;

    memory.availableBytes =
        status.ullAvailPhys;

    memory.usedBytes =
        memory.totalBytes -
        memory.availableBytes;

    return memory;
}