#include "JsonSerializer.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string monitoringDataToJson(
    const MonitoringData& data
)
{
    json root;

    root["cpu"] =
    {
        {"name", data.cpuName},
        {"usagePercent", data.cpuUsagePercent},
        {"physicalCores", data.physicalCores},
        {"logicalProcessors", data.logicalProcessors}
    };

    root["memory"] =
    {
        {"totalGB", data.ramTotalGB},
        {"usedGB", data.ramUsedGB},
        {"availableGB", data.ramAvailableGB},
        {"usagePercent", data.ramUsagePercent}
    };

    root["gpu"] =
    {
        {"available", data.gpuAvailable},
        {"name", data.gpuName},
        {"usagePercent", data.gpuUsagePercent},
        {"temperature", data.gpuTemperature},
        {"vramTotalGB", data.vramTotalGB},
        {"vramUsedGB", data.vramUsedGB},
        {"vramFreeGB", data.vramFreeGB},
        {"coreClockMHz", data.gpuCoreClockMHz},
        {"memoryClockMHz", data.gpuMemoryClockMHz}
    };

    root["windows"] =
    {
        {"name", data.windowsName},
        {"version", data.windowsVersion},
        {"build", data.windowsBuild},
        {"architecture", data.windowsArchitecture}
    };

    return root.dump(4);
}