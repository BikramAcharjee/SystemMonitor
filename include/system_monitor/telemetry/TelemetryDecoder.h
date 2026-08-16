#pragma once

#include <cstdint>
#include <vector>

#include "system_monitor/telemetry/TelemetryPacket.h"

class TelemetryDecoder
{
public:
    static TelemetryPacket decode(
        const std::vector<std::uint8_t>& buffer
    );
};