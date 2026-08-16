#pragma once

#include <cstdint>
#include <string>

struct DiskInfo
{
    std::string drive;

    std::uint64_t totalBytes = 0;
    std::uint64_t freeBytes = 0;
    std::uint64_t usedBytes = 0;

    double totalGB() const;
    double freeGB() const;
    double usedGB() const;
    double usagePercent() const;
};