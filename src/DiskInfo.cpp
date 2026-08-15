#include "DiskInfo.h"

#include <windows.h>

namespace
{
    constexpr double BYTES_PER_GB =
        1024.0 * 1024.0 * 1024.0;
}

double DiskInfo::totalGB() const
{
    return static_cast<double>(totalBytes)
        / BYTES_PER_GB;
}

double DiskInfo::freeGB() const
{
    return static_cast<double>(freeBytes)
        / BYTES_PER_GB;
}

double DiskInfo::usedGB() const
{
    return static_cast<double>(usedBytes)
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

std::vector<DiskInfo> getDiskInfo()
{
    std::vector<DiskInfo> disks;

    DWORD drives = GetLogicalDrives();

    if (drives == 0)
    {
        return disks;
    }

    for (char letter = 'A'; letter <= 'Z'; ++letter)
    {
        DWORD mask = 1 << (letter - 'A');

        if ((drives & mask) == 0)
        {
            continue;
        }

        std::string root;

        root += letter;
        root += ":\\";


        UINT driveType =
            GetDriveTypeA(root.c_str());

        // Only monitor fixed disks.
        if (driveType != DRIVE_FIXED)
        {
            continue;
        }

        ULARGE_INTEGER freeBytesAvailable;
        ULARGE_INTEGER totalBytes;
        ULARGE_INTEGER totalFreeBytes;

        if (!GetDiskFreeSpaceExA(
            root.c_str(),
            &freeBytesAvailable,
            &totalBytes,
            &totalFreeBytes))
        {
            continue;
        }

        DiskInfo disk;

        disk.drive = root;

        disk.totalBytes =
            totalBytes.QuadPart;

        disk.freeBytes =
            totalFreeBytes.QuadPart;

        disk.usedBytes =
            disk.totalBytes -
            disk.freeBytes;

        disks.push_back(disk);
    }

    return disks;
}