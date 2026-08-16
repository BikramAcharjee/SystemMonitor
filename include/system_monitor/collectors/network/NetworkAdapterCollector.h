#pragma once

#include "system_monitor/models/NetworkAdapterInfo.h"

#include <vector>

class NetworkAdapterCollector
{
public:

    std::vector<NetworkAdapterInfo> collect();
};