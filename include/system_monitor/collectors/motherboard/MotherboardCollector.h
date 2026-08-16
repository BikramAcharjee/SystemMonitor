#pragma once

#include "WmiClient.h"
#include "system_monitor/models/MotherboardInfo.h"

class MotherboardCollector
{
public:

    explicit MotherboardCollector(WmiClient& wmi);

    MotherboardInfo collect();

private:

    WmiClient& wmi;
};