#pragma once

class CpuUsage
{
public:
    double getUsage();

private:
    bool initialized_ = false;

    unsigned long long previousIdle_ = 0;
    unsigned long long previousKernel_ = 0;
    unsigned long long previousUser_ = 0;
};