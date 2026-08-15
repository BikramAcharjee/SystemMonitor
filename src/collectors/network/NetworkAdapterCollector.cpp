#include "system_monitor/collectors/network/NetworkAdapterCollector.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <iphlpapi.h>

#include <iomanip>
#include <sstream>


#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

namespace
{
    std::string macAddressToString(
        const BYTE* address,
        ULONG length)
    {
        if (address == nullptr || length == 0)
        {
            return {};
        }

        std::ostringstream stream;

        stream << std::uppercase
            << std::hex
            << std::setfill('0');

        for (ULONG i = 0; i < length; ++i)
        {
            if (i > 0)
            {
                stream << ":";
            }

            stream << std::setw(2)
                << static_cast<int>(address[i]);
        }

        return stream.str();
    }


    std::string sockaddrToString(
        const SOCKADDR* address)
    {
        if (address == nullptr)
        {
            return {};
        }

        char buffer[INET6_ADDRSTRLEN]{};

        if (address->sa_family == AF_INET)
        {
            const auto* ipv4 =
                reinterpret_cast<const SOCKADDR_IN*>(address);

            if (InetNtopA(
                AF_INET,
                &ipv4->sin_addr,
                buffer,
                sizeof(buffer)) == nullptr)
            {
                return {};
            }
        }
        else if (address->sa_family == AF_INET6)
        {
            const auto* ipv6 =
                reinterpret_cast<const SOCKADDR_IN6*>(address);

            if (InetNtopA(
                AF_INET6,
                &ipv6->sin6_addr,
                buffer,
                sizeof(buffer)) == nullptr)
            {
                return {};
            }
        }
        else
        {
            return {};
        }

        return buffer;
    }
}


std::vector<NetworkAdapterInfo>
NetworkAdapterCollector::collect()
{
    std::vector<NetworkAdapterInfo> adapters;

    ULONG bufferSize = 0;

    DWORD result = GetAdaptersAddresses(
        AF_UNSPEC,
        GAA_FLAG_INCLUDE_PREFIX,
        nullptr,
        nullptr,
        &bufferSize
    );

    if (result != ERROR_BUFFER_OVERFLOW)
    {
        return adapters;
    }

    std::vector<BYTE> buffer(bufferSize);

    auto* addresses =
        reinterpret_cast<IP_ADAPTER_ADDRESSES*>(
            buffer.data()
            );

    result = GetAdaptersAddresses(
        AF_UNSPEC,
        GAA_FLAG_INCLUDE_PREFIX,
        nullptr,
        addresses,
        &bufferSize
    );

    if (result != NO_ERROR)
    {
        return adapters;
    }


    for (
        auto* adapter = addresses;
        adapter != nullptr;
        adapter = adapter->Next)
    {
        NetworkAdapterInfo info;


        // ----------------------------------------------------
        // Adapter name
        // ----------------------------------------------------

        if (adapter->FriendlyName != nullptr)
        {
            int size = WideCharToMultiByte(
                CP_UTF8,
                0,
                adapter->FriendlyName,
                -1,
                nullptr,
                0,
                nullptr,
                nullptr
            );

            if (size > 0)
            {
                std::string resultString(size - 1, '\0');

                WideCharToMultiByte(
                    CP_UTF8,
                    0,
                    adapter->FriendlyName,
                    -1,
                    resultString.data(),
                    size,
                    nullptr,
                    nullptr
                );

                info.name = resultString;
            }
        }


        // ----------------------------------------------------
        // Adapter description
        // ----------------------------------------------------

        if (adapter->Description != nullptr)
        {
            int size = WideCharToMultiByte(
                CP_UTF8,
                0,
                adapter->Description,
                -1,
                nullptr,
                0,
                nullptr,
                nullptr
            );

            if (size > 0)
            {
                std::string resultString(size - 1, '\0');

                WideCharToMultiByte(
                    CP_UTF8,
                    0,
                    adapter->Description,
                    -1,
                    resultString.data(),
                    size,
                    nullptr,
                    nullptr
                );

                info.description = resultString;
            }
        }


        // ----------------------------------------------------
        // MAC address
        // ----------------------------------------------------

        info.macAddress =
            macAddressToString(
                adapter->PhysicalAddress,
                adapter->PhysicalAddressLength
            );


        // ----------------------------------------------------
        // Connection status
        // ----------------------------------------------------

        info.connected =
            adapter->OperStatus == IfOperStatusUp;


        // ----------------------------------------------------
        // IP addresses
        // ----------------------------------------------------

        for (
            auto* address = adapter->FirstUnicastAddress;
            address != nullptr;
            address = address->Next)
        {
            std::string ip =
                sockaddrToString(
                    address->Address.lpSockaddr
                );

            if (address->Address.lpSockaddr->sa_family
                == AF_INET)
            {
                if (info.ipv4Address.empty())
                {
                    info.ipv4Address = ip;
                }
            }
            else if (
                address->Address.lpSockaddr->sa_family
                == AF_INET6)
            {
                if (info.ipv6Address.empty())
                {
                    info.ipv6Address = ip;
                }
            }
        }


        adapters.push_back(info);
    }

    return adapters;
}