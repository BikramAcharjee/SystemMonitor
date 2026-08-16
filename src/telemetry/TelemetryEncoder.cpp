#include "system_monitor/telemetry/TelemetryEncoder.h"

#include <cstring>

namespace
{
    void writeUint16(
        std::vector<std::uint8_t>& buffer,
        std::uint16_t value)
    {
        buffer.push_back(
            static_cast<std::uint8_t>(value & 0xFF)
        );

        buffer.push_back(
            static_cast<std::uint8_t>((value >> 8) & 0xFF)
        );
    }

    void writeUint64(
        std::vector<std::uint8_t>& buffer,
        std::uint64_t value)
    {
        for (int i = 0; i < 8; ++i)
        {
            buffer.push_back(
                static_cast<std::uint8_t>(
                    (value >> (i * 8)) & 0xFF
                    )
            );
        }
    }

    void writeFloat(
        std::vector<std::uint8_t>& buffer,
        float value)
    {
        static_assert(sizeof(float) == sizeof(std::uint32_t));

        std::uint32_t rawValue = 0;

        std::memcpy(
            &rawValue,
            &value,
            sizeof(float)
        );

        buffer.push_back(
            static_cast<std::uint8_t>(rawValue & 0xFF)
        );

        buffer.push_back(
            static_cast<std::uint8_t>((rawValue >> 8) & 0xFF)
        );

        buffer.push_back(
            static_cast<std::uint8_t>((rawValue >> 16) & 0xFF)
        );

        buffer.push_back(
            static_cast<std::uint8_t>((rawValue >> 24) & 0xFF)
        );
    }
}

std::vector<std::uint8_t> TelemetryEncoder::encode(
    const TelemetryPacket& packet)
{
    std::vector<std::uint8_t> buffer;

    buffer.reserve(18);

    writeUint16(buffer, packet.packetId);
    writeUint64(buffer, packet.timestamp);
    writeFloat(buffer, packet.cpuUsage);
    writeFloat(buffer, packet.memoryUsage);

    return buffer;
}