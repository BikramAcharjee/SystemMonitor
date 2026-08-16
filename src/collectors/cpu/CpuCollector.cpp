#include "system_monitor/collectors/cpu/CpuCollector.h"

#include "system_monitor/WmiClient.h"

CpuCollector::CpuCollector(WmiClient& wmi)
    : wmi(wmi)
{
}

CpuInfo CpuCollector::collect()
{
    CpuInfo cpu;

    cpu.name = wmi.getProperty(
        L"SELECT Name FROM Win32_Processor",
        L"Name"
    );

    cpu.manufacturer = wmi.getProperty(
        L"SELECT Manufacturer FROM Win32_Processor",
        L"Manufacturer"
    );

    std::string cores = wmi.getProperty(
        L"SELECT NumberOfCores FROM Win32_Processor",
        L"NumberOfCores"
    );

    std::string threads = wmi.getProperty(
        L"SELECT NumberOfLogicalProcessors FROM Win32_Processor",
        L"NumberOfLogicalProcessors"
    );

    try
    {
        cpu.physicalCores = std::stoul(cores);
        cpu.logicalProcessors = std::stoul(threads);
    }
    catch (...)
    {
        cpu.physicalCores = 0;
        cpu.logicalProcessors = 0;
    }

    return cpu;
}