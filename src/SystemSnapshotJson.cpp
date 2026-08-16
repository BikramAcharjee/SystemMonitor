#include "SystemSnapshotJson.h"

nlohmann::json toJson(const SystemSnapshot& snapshot)
{
    nlohmann::json json;

    // --------------------------------------------------------
    // Metadata
    // --------------------------------------------------------

    json["timestamp"] = snapshot.timestamp;

    // --------------------------------------------------------
    // CPU
    // --------------------------------------------------------

    json["cpu"] = {
        {"name", snapshot.cpu.name},
        {"manufacturer", snapshot.cpu.manufacturer},
        {"usagePercent", snapshot.cpuUsagePercent},
        {"physicalCores", snapshot.cpu.physicalCores},
        {"logicalProcessors", snapshot.cpu.logicalProcessors}
    };

    // --------------------------------------------------------
    // Memory
    // --------------------------------------------------------

    json["memory"] = {
        {"totalBytes", snapshot.memory.totalBytes},
        {"availableBytes", snapshot.memory.availableBytes},
        {"usedBytes", snapshot.memory.usedBytes},
        {"totalGB", snapshot.memory.totalGB()},
        {"availableGB", snapshot.memory.availableGB()},
        {"usedGB", snapshot.memory.usedGB()},
        {"usagePercent", snapshot.memory.usagePercent()}
    };

    // --------------------------------------------------------
    // GPU
    // --------------------------------------------------------

    json["gpu"] = {
        {"available", snapshot.gpu.available},
        {"name", snapshot.gpu.name},
        {"utilizationPercent", snapshot.gpu.utilizationPercent},
        {"temperatureC", snapshot.gpu.temperature},
        {"memoryTotalBytes", snapshot.gpu.memoryTotal},
        {"memoryUsedBytes", snapshot.gpu.memoryUsed},
        {"memoryFreeBytes", snapshot.gpu.memoryFree},
        {"memoryTotalGB", snapshot.gpu.memoryTotalGB()},
        {"memoryUsedGB", snapshot.gpu.memoryUsedGB()},
        {"memoryFreeGB", snapshot.gpu.memoryFreeGB()},
        {"coreClockMHz", snapshot.gpu.coreClockMHz},
        {"memoryClockMHz", snapshot.gpu.memoryClockMHz}
    };

    // --------------------------------------------------------
    // Motherboard
    // --------------------------------------------------------

    json["motherboard"] = {
        {"manufacturer", snapshot.motherboard.manufacturer},
        {"model", snapshot.motherboard.model},
        {"serialNumber", snapshot.motherboard.serialNumber}
    };

    // --------------------------------------------------------
    // Windows
    // --------------------------------------------------------

    json["windows"] = {
        {"name", snapshot.windows.name},
        {"version", snapshot.windows.version},
        {"buildNumber", snapshot.windows.buildNumber},
        {"architecture", snapshot.windows.architecture}
    };

    // --------------------------------------------------------
    // Disks
    // --------------------------------------------------------

    json["disks"] = nlohmann::json::array();

    for (const auto& disk : snapshot.disks)
    {
        json["disks"].push_back({
            {"drive", disk.drive},
            {"totalBytes", disk.totalBytes},
            {"freeBytes", disk.freeBytes},
            {"usedBytes", disk.usedBytes},
            {"totalGB", disk.totalGB()},
            {"freeGB", disk.freeGB()},
            {"usedGB", disk.usedGB()},
            {"usagePercent", disk.usagePercent()}
            });
    }

    // --------------------------------------------------------
    // Network
    // --------------------------------------------------------

    json["network"] = nlohmann::json::array();

    for (const auto& adapter : snapshot.network)
    {
        json["network"].push_back({
            {"adapterName", adapter.adapterName},
            {"description", adapter.description},
            {"connected", adapter.connected},
            {"receivedBytes", adapter.receivedBytes},
            {"sentBytes", adapter.sentBytes},
            {"downloadMBps", adapter.downloadMBps},
            {"uploadMBps", adapter.uploadMBps}
            });
    }

    return json;
}

nlohmann::json cpuToJson(
    const SystemSnapshot& snapshot)
{
    return {
        {"name", snapshot.cpu.name},
        {"manufacturer", snapshot.cpu.manufacturer},
        {"usagePercent", snapshot.cpuUsagePercent},
        {"physicalCores", snapshot.cpu.physicalCores},
        {"logicalProcessors", snapshot.cpu.logicalProcessors}
    };
}

nlohmann::json memoryToJson(
    const SystemSnapshot& snapshot)
{
    return {
        {"totalBytes", snapshot.memory.totalBytes},
        {"availableBytes", snapshot.memory.availableBytes},
        {"usedBytes", snapshot.memory.usedBytes},
        {"totalGB", snapshot.memory.totalGB()},
        {"availableGB", snapshot.memory.availableGB()},
        {"usedGB", snapshot.memory.usedGB()},
        {"usagePercent", snapshot.memory.usagePercent()}
    };
}

nlohmann::json gpuToJson(
    const SystemSnapshot& snapshot)
{
    return {
        {"available", snapshot.gpu.available},
        {"name", snapshot.gpu.name},
        {"utilizationPercent", snapshot.gpu.utilizationPercent},
        {"temperatureC", snapshot.gpu.temperature},
        {"memoryTotalBytes", snapshot.gpu.memoryTotal},
        {"memoryUsedBytes", snapshot.gpu.memoryUsed},
        {"memoryFreeBytes", snapshot.gpu.memoryFree},
        {"memoryTotalGB", snapshot.gpu.memoryTotalGB()},
        {"memoryUsedGB", snapshot.gpu.memoryUsedGB()},
        {"memoryFreeGB", snapshot.gpu.memoryFreeGB()},
        {"coreClockMHz", snapshot.gpu.coreClockMHz},
        {"memoryClockMHz", snapshot.gpu.memoryClockMHz}
    };
}

nlohmann::json disksToJson(
    const SystemSnapshot& snapshot)
{
    nlohmann::json disks =
        nlohmann::json::array();

    for (const auto& disk : snapshot.disks)
    {
        disks.push_back({
            {"drive", disk.drive},
            {"totalBytes", disk.totalBytes},
            {"freeBytes", disk.freeBytes},
            {"usedBytes", disk.usedBytes},
            {"totalGB", disk.totalGB()},
            {"freeGB", disk.freeGB()},
            {"usedGB", disk.usedGB()},
            {"usagePercent", disk.usagePercent()}
            });
    }

    return disks;
}

nlohmann::json networkToJson(
    const SystemSnapshot& snapshot)
{
    nlohmann::json network =
        nlohmann::json::array();

    for (const auto& adapter : snapshot.network)
    {
        network.push_back({
            {"adapterName", adapter.adapterName},
            {"description", adapter.description},
            {"connected", adapter.connected},
            {"receivedBytes", adapter.receivedBytes},
            {"sentBytes", adapter.sentBytes},
            {"downloadMBps", adapter.downloadMBps},
            {"uploadMBps", adapter.uploadMBps}
            });
    }

    return network;
}