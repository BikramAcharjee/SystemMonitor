#pragma once

#include <vector>

#include "system_monitor/models/DiskInfo.h"

class DiskCollector
{
public:

    std::vector<DiskInfo> collect();
};