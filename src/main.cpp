#include <iostream>
#include <nlohmann/json.hpp>
#include <httplib.h>

#include "WmiClient.h"
#include "MonitorEngine.h"
#include "SystemSnapshotJson.h"


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

                res.set_content(
                    json.dump(4),
                    "application/json"
                );
            }
            catch (const std::exception& ex)
            {
                nlohmann::json error =
                {
                    {"error", ex.what()}
                };

                res.status = 500;

                res.set_content(
                    error.dump(4),
                    "application/json"
                );
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
        << "\nPress Ctrl+C to stop.\n";


    server.listen(
        "0.0.0.0",
        8080
    );


    return 0;
}