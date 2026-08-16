#pragma once

#include "system_monitor/models/CpuInfo.h"

class WmiClient;

class CpuCollector
{
public:
    explicit CpuCollector(WmiClient& wmi);

    CpuInfo collect();

private:
    WmiClient& wmi;
};