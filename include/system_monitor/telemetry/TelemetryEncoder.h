#pragma once

#include <cstdint>
#include <vector>

#include "system_monitor/telemetry/TelemetryPacket.h"

class TelemetryEncoder
{
public:
    static std::vector<std::uint8_t> encode(
        const TelemetryPacket& packet
    );
};