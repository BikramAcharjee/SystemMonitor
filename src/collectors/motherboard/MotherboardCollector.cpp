#include "system_monitor/collectors/motherboard/MotherboardCollector.h"

MotherboardCollector::MotherboardCollector(
    WmiClient& wmi)
    : wmi(wmi)
{
}

MotherboardInfo
MotherboardCollector::collect()
{
    MotherboardInfo motherboard;

    motherboard.manufacturer =
        wmi.getProperty(
            L"SELECT Manufacturer FROM Win32_BaseBoard",
            L"Manufacturer"
        );

    motherboard.model =
        wmi.getProperty(
            L"SELECT Product FROM Win32_BaseBoard",
            L"Product"
        );

    motherboard.serialNumber =
        wmi.getProperty(
            L"SELECT SerialNumber FROM Win32_BaseBoard",
            L"SerialNumber"
        );

    return motherboard;
}