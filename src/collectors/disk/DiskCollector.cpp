#include "system_monitor/collectors/disk/DiskCollector.h"

#include <windows.h>

std::vector<DiskInfo> DiskCollector::collect()
{
    std::vector<DiskInfo> disks;

    DWORD drives = GetLogicalDrives();

    if (drives == 0)
    {
        return disks;
    }

    for (char letter = 'A'; letter <= 'Z'; ++letter)
    {
        DWORD mask =
            1 << (letter - 'A');

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

        ULARGE_INTEGER freeBytesAvailable{};
        ULARGE_INTEGER totalBytes{};
        ULARGE_INTEGER totalFreeBytes{};

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