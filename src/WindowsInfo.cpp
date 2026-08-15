#include "WindowsInfo.h"
#include "WmiClient.h"

WindowsInfo getWindowsInfo(WmiClient& wmi)
{
    WindowsInfo windows;

    windows.name = wmi.getProperty(
        L"SELECT Caption FROM Win32_OperatingSystem",
        L"Caption"
    );

    windows.version = wmi.getProperty(
        L"SELECT Version FROM Win32_OperatingSystem",
        L"Version"
    );

    windows.buildNumber = wmi.getProperty(
        L"SELECT BuildNumber FROM Win32_OperatingSystem",
        L"BuildNumber"
    );

    windows.architecture = wmi.getProperty(
        L"SELECT OSArchitecture FROM Win32_OperatingSystem",
        L"OSArchitecture"
    );

    return windows;
}