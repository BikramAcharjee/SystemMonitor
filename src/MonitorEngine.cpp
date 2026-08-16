#define NOMINMAX

#include "system_monitor/MonitorEngine.h"
#include "system_monitor/utils/TimeUtils.h"


MonitorEngine::MonitorEngine(WmiClient& wmi)
    : cpuCollector(wmi), motherboardCollector(wmi), windowsCollector(wmi)
{
    collectStaticInformation();
}

SystemSnapshot MonitorEngine::createSnapshot()
{
    SystemSnapshot snapshot;

    snapshot.timestamp = getCurrentTimestamp();

    snapshot.cpu = cpu;
    snapshot.cpuUsagePercent = cpuUsage.getUsage();

    snapshot.memory = memoryCollector.collect();

    snapshot.gpu = gpuCollector.collect();

    snapshot.disks = diskCollector.collect();

    snapshot.network = networkTrafficCollector.collect();

    snapshot.motherboard = motherboard;

    snapshot.windows = windows;

    return snapshot;
}


// ============================================================
// Collect information that doesn't change frequently
// ============================================================

void MonitorEngine::collectStaticInformation()
{
    cpu = cpuCollector.collect();

    motherboard = motherboardCollector.collect();

    windows = windowsCollector.collect();
}