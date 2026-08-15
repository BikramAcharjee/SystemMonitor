#pragma once

#include <string>

class WmiClient;

struct CpuInfo
{
    std::string name;
    std::string manufacturer;
    unsigned int physicalCores = 0;
    unsigned int logicalProcessors = 0;
};

CpuInfo getCpuInfo(WmiClient& wmi);