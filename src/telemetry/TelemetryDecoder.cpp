#include "system_monitor/telemetry/TelemetryDecoder.h"

#include <cstring>
#include <stdexcept>

namespace
{
    std::uint16_t readUint16(
        const std::vector<std::uint8_t>& buffer,
        std::size_t offset)
    {
        return static_cast<std::uint16_t>(
            buffer[offset] |
            (static_cast<std::uint16_t>(buffer[offset + 1]) << 8)
            );
    }

    std::uint64_t readUint64(
        const std::vector<std::uint8_t>& buffer,
        std::size_t offset)
    {
        std::uint64_t value = 0;

        for (int i = 0; i < 8; ++i)
        {
            value |=
                static_cast<std::uint64_t>(buffer[offset + i])
                << (i * 8);
        }

        return value;
    }

    float readFloat(
        const std::vector<std::uint8_t>& buffer,
        std::size_t offset)
    {
        std::uint32_t rawValue =
            static_cast<std::uint32_t>(buffer[offset]) |
            (static_cast<std::uint32_t>(buffer[offset + 1]) << 8) |
            (static_cast<std::uint32_t>(buffer[offset + 2]) << 16) |
            (static_cast<std::uint32_t>(buffer[offset + 3]) << 24);

        float value = 0.0f;

        std::memcpy(
            &value,
            &rawValue,
            sizeof(float)
        );

        return value;
    }
}

TelemetryPacket TelemetryDecoder::decode(
    const std::vector<std::uint8_t>& buffer)
{
    if (buffer.size() != 18)
    {
        throw std::invalid_argument(
            "Invalid telemetry packet size"
        );
    }

    TelemetryPacket packet;

    packet.packetId = readUint16(buffer, 0);
    packet.timestamp = readUint64(buffer, 2);
    packet.cpuUsage = readFloat(buffer, 10);
    packet.memoryUsage = readFloat(buffer, 14);

    return packet;
}