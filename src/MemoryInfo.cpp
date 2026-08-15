#include "MemoryInfo.h"

#include <windows.h>

double MemoryInfo::totalGB() const
{
    return static_cast<double>(totalBytes)
        / (1024.0 * 1024.0 * 1024.0);
}

MemoryInfo getMemoryInfo()
{
    MemoryInfo memory;

    MEMORYSTATUSEX status;

    status.dwLength = sizeof(status);

    if (GlobalMemoryStatusEx(&status))
    {
        memory.totalBytes = status.ullTotalPhys;
    }

    return memory;
}