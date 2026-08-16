#pragma once

#include <string>

struct CpuInfo
{
    std::string name;
    std::string manufacturer;

    unsigned int physicalCores = 0;
    unsigned int logicalProcessors = 0;
};