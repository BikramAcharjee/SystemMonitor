#pragma once

#include "system_monitor/models/MemoryInfo.h"

class MemoryCollector
{
public:
    MemoryInfo collect();
};