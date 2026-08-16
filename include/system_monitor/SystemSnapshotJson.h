#pragma once

#include "system_monitor/SystemSnapshot.h"

#include <nlohmann/json.hpp>

nlohmann::json toJson(
    const SystemSnapshot& snapshot
);

nlohmann::json cpuToJson(
    const SystemSnapshot& snapshot
);

nlohmann::json memoryToJson(
    const SystemSnapshot& snapshot
);

nlohmann::json gpuToJson(
    const SystemSnapshot& snapshot
);

nlohmann::json disksToJson(
    const SystemSnapshot& snapshot
);

nlohmann::json networkToJson(
    const SystemSnapshot& snapshot
);