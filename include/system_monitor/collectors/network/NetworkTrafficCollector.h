#pragma once

#include "system_monitor/models/NetworkTraffic.h"

#include <vector>

class NetworkTrafficCollector
{
public:

    std::vector<NetworkTraffic> collect();
};