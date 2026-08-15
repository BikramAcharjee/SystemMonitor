#pragma once

#include <string>

class WmiClient;

struct MotherboardInfo
{
    std::string manufacturer;
    std::string model;
    std::string serialNumber;
};

MotherboardInfo getMotherboardInfo(WmiClient& wmi);