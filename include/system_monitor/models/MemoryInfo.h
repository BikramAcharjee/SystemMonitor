#pragma once

#include <cstdint>

struct MemoryInfo
{
    std::uint64_t totalBytes = 0;
    std::uint64_t availableBytes = 0;
    std::uint64_t usedBytes = 0;

    double usagePercent() const;
    double totalGB() const;
    double usedGB() const;
    double availableGB() const;
};