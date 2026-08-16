#include "system_monitor/collectors/memory/MemoryCollector.h"

#include <windows.h>

MemoryInfo MemoryCollector::collect()
{
    MemoryInfo memory;

    MEMORYSTATUSEX status{};

    status.dwLength = sizeof(status);

    if (!GlobalMemoryStatusEx(&status))
    {
        return memory;
    }

    memory.totalBytes = status.ullTotalPhys;

    memory.availableBytes = status.ullAvailPhys;

    memory.usedBytes = memory.totalBytes - memory.availableBytes;

    return memory;
}