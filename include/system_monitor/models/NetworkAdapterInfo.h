#pragma once

#include <cstdint>
#include <string>

struct NetworkAdapterInfo
{
    std::string name;
    std::string description;
    std::string macAddress;
    std::string ipv4Address;
    std::string ipv6Address;

    bool connected = false;

    std::uint64_t receivedBytes = 0;
    std::uint64_t sentBytes = 0;

    double downloadMbps = 0.0;
    double uploadMbps = 0.0;
};