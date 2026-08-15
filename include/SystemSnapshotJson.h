#pragma once

#include "SystemSnapshot.h"
#include <nlohmann/json.hpp>

nlohmann::json toJson(const SystemSnapshot& snapshot);