#pragma once

#include "WmiClient.h"
#include "system_monitor/models/WindowsInfo.h"

class WindowsCollector
{
public:

    explicit WindowsCollector(WmiClient& wmi);

    WindowsInfo collect();

private:

    WmiClient& wmi;
};