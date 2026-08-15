#pragma once

#include <cstdint>

struct MemoryInfo
{
    std::uint64_t totalBytes = 0;

    double totalGB() const;
};

MemoryInfo getMemoryInfo();