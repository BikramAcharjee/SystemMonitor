#include "system_monitor/collectors/network/NetworkTrafficCollector.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <ws2def.h>
#include <ws2ipdef.h>

#include <iphlpapi.h>

#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>
#include <chrono>

#pragma comment(lib, "iphlpapi.lib")


namespace
{
    struct PreviousCounters
    {
        std::uint64_t receivedBytes = 0;
        std::uint64_t sentBytes = 0;

        std::chrono::steady_clock::time_point timestamp;
    };


    std::unordered_map<
        NET_IFINDEX,
        PreviousCounters
    > previousCounters;


    // --------------------------------------------------------
    // Windows WCHAR -> UTF-8 std::string
    // --------------------------------------------------------

    std::string wideToString(
        const WCHAR* value)
    {
        if (value == nullptr ||
            value[0] == L'\0')
        {
            return "";
        }


        int requiredSize =
            WideCharToMultiByte(
                CP_UTF8,
                0,
                value,
                -1,
                nullptr,
                0,
                nullptr,
                nullptr
            );


        if (requiredSize <= 0)
        {
            return "";
        }


        std::string result(
            requiredSize,
            '\0'
        );


        WideCharToMultiByte(
            CP_UTF8,
            0,
            value,
            -1,
            result.data(),
            requiredSize,
            nullptr,
            nullptr
        );


        if (!result.empty() &&
            result.back() == '\0')
        {
            result.pop_back();
        }


        return result;
    }
}


// ============================================================
// NetworkTrafficMonitor::collect
// ============================================================

std::vector<NetworkTraffic>
NetworkTrafficCollector::collect()
{
    std::vector<NetworkTraffic> result;


    PMIB_IF_TABLE2 table = nullptr;


    // --------------------------------------------------------
    // Get complete network interface table
    // --------------------------------------------------------

    DWORD status =
        GetIfTable2(
            &table
        );


    if (status != NO_ERROR)
    {
        return result;
    }


    const auto now =
        std::chrono::steady_clock::now();


    // --------------------------------------------------------
    // Process every interface
    // --------------------------------------------------------

    for (ULONG i = 0;
        i < table->NumEntries;
        ++i)
    {
        const MIB_IF_ROW2& row =
            table->Table[i];


        // ----------------------------------------------------
        // Ignore interfaces that are not operational
        // ----------------------------------------------------

        if (row.OperStatus !=
            IfOperStatusUp)
        {
            continue;
        }

        // --------------------------------------------------------
        // Only monitor real hardware interfaces
        // --------------------------------------------------------

        if (!row.InterfaceAndOperStatusFlags.HardwareInterface)
        {
            continue;
        }



        NetworkTraffic traffic;


        // ----------------------------------------------------
        // Adapter name
        // ----------------------------------------------------

        traffic.adapterName =
            wideToString(
                row.Alias
            );


        // ----------------------------------------------------
        // Adapter description
        // ----------------------------------------------------

        traffic.description =
            wideToString(
                row.Description
            );


        // ----------------------------------------------------
        // Connection state
        // ----------------------------------------------------

        traffic.connected = true;


        // ----------------------------------------------------
        // Current 64-bit counters
        // ----------------------------------------------------

        traffic.receivedBytes =
            row.InOctets;


        traffic.sentBytes =
            row.OutOctets;


        // ----------------------------------------------------
        // Find previous measurement
        // ----------------------------------------------------

        auto iterator =
            previousCounters.find(
                row.InterfaceIndex
            );


        if (iterator !=
            previousCounters.end())
        {
            const PreviousCounters&
                previous =
                iterator->second;


            double elapsedSeconds =
                std::chrono::duration<double>(
                    now -
                    previous.timestamp
                ).count();


            if (elapsedSeconds > 0.0)
            {
                // --------------------------------------------
                // Download
                // --------------------------------------------

                if (traffic.receivedBytes >=
                    previous.receivedBytes)
                {
                    std::uint64_t delta =
                        traffic.receivedBytes -
                        previous.receivedBytes;


                    traffic.downloadMBps =
                        (
                            static_cast<double>(
                                delta
                                )
                            /
                            (1024.0 * 1024.0)
                            )
                        /
                        elapsedSeconds;
                }


                // --------------------------------------------
                // Upload
                // --------------------------------------------

                if (traffic.sentBytes >=
                    previous.sentBytes)
                {
                    std::uint64_t delta =
                        traffic.sentBytes -
                        previous.sentBytes;


                    traffic.uploadMBps =
                        (
                            static_cast<double>(
                                delta
                                )
                            /
                            (1024.0 * 1024.0)
                            )
                        /
                        elapsedSeconds;
                }
            }
        }


        // ----------------------------------------------------
        // Store current counters
        // ----------------------------------------------------

        previousCounters[
            row.InterfaceIndex
        ] =
            {
                traffic.receivedBytes,
                traffic.sentBytes,
                now
            };


            // ----------------------------------------------------
            // Add result
            // ----------------------------------------------------

            result.push_back(
                traffic
            );
    }


    // --------------------------------------------------------
    // Release Windows table
    // --------------------------------------------------------

    FreeMibTable(
        table
    );


    return result;
}