#pragma once

#include <cstdint>

struct TelemetryPacket
{
    std::uint16_t packetId = 0;

    std::uint64_t timestamp = 0;

    float cpuUsage = 0.0f;

    float memoryUsage = 0.0f;
};