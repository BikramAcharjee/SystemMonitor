#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct NetworkTraffic
{
    std::string adapterName;
    std::string description;

    bool connected = false;

    std::uint64_t receivedBytes = 0;
    std::uint64_t sentBytes = 0;

    double downloadMBps = 0.0;
    double uploadMBps = 0.0;
};


class NetworkTrafficMonitor
{
public:

    std::vector<NetworkTraffic>
        sample();
};