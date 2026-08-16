#include "system_monitor/collectors/cpu/CpuUsage.h"

#include <windows.h>

double CpuUsage::getUsage()
{
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;

    if (!GetSystemTimes(
        &idleTime,
        &kernelTime,
        &userTime))
    {
        return 0.0;
    }

    ULARGE_INTEGER idle;
    ULARGE_INTEGER kernel;
    ULARGE_INTEGER user;

    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;

    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;

    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;


    unsigned long long currentIdle =
        idle.QuadPart;

    unsigned long long currentKernel =
        kernel.QuadPart;

    unsigned long long currentUser =
        user.QuadPart;


    // First call establishes baseline.
    if (!initialized_)
    {
        previousIdle_ = currentIdle;
        previousKernel_ = currentKernel;
        previousUser_ = currentUser;

        initialized_ = true;

        return 0.0;
    }


    unsigned long long idleDelta =
        currentIdle - previousIdle_;

    unsigned long long kernelDelta =
        currentKernel - previousKernel_;

    unsigned long long userDelta =
        currentUser - previousUser_;


    unsigned long long totalDelta =
        kernelDelta + userDelta;


    // Kernel time includes idle time.
    unsigned long long busyDelta =
        totalDelta - idleDelta;


    previousIdle_ = currentIdle;
    previousKernel_ = currentKernel;
    previousUser_ = currentUser;


    if (totalDelta == 0)
    {
        return 0.0;
    }


    double usage =
        (static_cast<double>(busyDelta)
            / static_cast<double>(totalDelta))
        * 100.0;


    return usage;
}