#include <iostream>
#include <nlohmann/json.hpp>
#include <httplib.h>

#include "WmiClient.h"
#include "MonitorEngine.h"
#include "SystemSnapshotJson.h"
#include "system_monitor/api/ApiResponse.h"


int main()
{
    std::cout
        << "========================================\n"
        << "       C++ SYSTEM MONITOR API\n"
        << "========================================\n";


    // --------------------------------------------------------
    // Initialize WMI
    // --------------------------------------------------------

    WmiClient wmi;

    if (!wmi.initialize())
    {
        std::cerr
            << "Failed to initialize WMI.\n";

        return 1;
    }


    // --------------------------------------------------------
    // Initialize monitoring engine
    // --------------------------------------------------------

    MonitorEngine monitor(wmi);


    // --------------------------------------------------------
    // Create HTTP server
    // --------------------------------------------------------

    httplib::Server server;


    // --------------------------------------------------------
    // System information endpoint
    // --------------------------------------------------------

    server.Get("/api/system",
        [&monitor](const httplib::Request& req,
            httplib::Response& res)
        {
            try
            {
                SystemSnapshot snapshot =
                    monitor.createSnapshot();

                nlohmann::json json =
                    toJson(snapshot);

                ApiResponse::json(res, json);
            }
            catch (const std::exception& ex)
            {
                ApiResponse::error(res, 500, ex.what());
            }
        });

    server.Get("/api/system/cpu",
        [&monitor](const httplib::Request& req,
            httplib::Response& res)
        {
            try
            {
                SystemSnapshot snapshot =
                    monitor.createSnapshot();

                nlohmann::json json =
                    cpuToJson(snapshot);

                ApiResponse::json(res,json);
            }
            catch (const std::exception& ex)
            {
                ApiResponse::error(res,500,ex.what());
            }
        });

    server.Get("/api/system/memory",
        [&monitor](const httplib::Request& req,
            httplib::Response& res)
        {
            try
            {
                SystemSnapshot snapshot =
                    monitor.createSnapshot();

                nlohmann::json json =
                    memoryToJson(snapshot);

                ApiResponse::json(res, json);
            }
            catch (const std::exception& ex)
            {
                ApiResponse::error(res, 500, ex.what());
            }
        });

    server.Get("/api/system/gpu",
        [&monitor](const httplib::Request& req,
            httplib::Response& res)
        {
            try
            {
                SystemSnapshot snapshot =
                    monitor.createSnapshot();

                nlohmann::json json =
                    gpuToJson(snapshot);

                ApiResponse::json(res, json);
            }
            catch (const std::exception& ex)
            {
                ApiResponse::error(res, 500, ex.what());
            }
        });

    server.Get("/api/system/storage",
        [&monitor](const httplib::Request& req,
            httplib::Response& res)
        {
            try
            {
                SystemSnapshot snapshot =
                    monitor.createSnapshot();

                nlohmann::json json =
                    disksToJson(snapshot);

                ApiResponse::json(res, json);
            }
            catch (const std::exception& ex)
            {
                ApiResponse::error(res, 500, ex.what());
            }
        });

    server.Get("/api/system/network",
        [&monitor](const httplib::Request& req,
            httplib::Response& res)
        {
            try
            {
                SystemSnapshot snapshot =
                    monitor.createSnapshot();

                nlohmann::json json =
                    networkToJson(snapshot);

                ApiResponse::json(res, json);
            }
            catch (const std::exception& ex)
            {
                ApiResponse::error(res, 500, ex.what());
            }
        });


    // --------------------------------------------------------
    // Health check
    // --------------------------------------------------------

    server.Get("/api/health",
        [](const httplib::Request& req,
            httplib::Response& res)
        {
            nlohmann::json response =
            {
                {"status", "UP"},
                {"service", "SystemMonitor"}
            };

            res.set_content(
                response.dump(4),
                "application/json"
            );
        });


    // --------------------------------------------------------
    // Start server
    // --------------------------------------------------------

    std::cout
        << "\nSystem Monitor API started.\n";

    std::cout
        << "Health : http://localhost:8080/api/health\n";

    std::cout
        << "System : http://localhost:8080/api/system\n";

    std::cout
        << "CPU    : http://localhost:8080/api/system/cpu\n";

    std::cout
        << "Memory : http://localhost:8080/api/system/memory\n";

    std::cout
        << "GPU    : http://localhost:8080/api/system/gpu\n";

    std::cout
        << "Storage: http://localhost:8080/api/system/storage\n";

    std::cout
        << "Network: http://localhost:8080/api/system/network\n";

    std::cout
        << "\nPress Ctrl+C to stop.\n";


    server.listen(
        "0.0.0.0",
        8080
    );


    return 0;
}