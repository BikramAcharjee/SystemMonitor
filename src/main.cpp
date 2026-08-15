#include <iostream>

#include "WmiClient.h"
#include "MonitorEngine.h"


int main()
{
    std::cout
        << "========================================\n";

    std::cout
        << "       C++ SYSTEM MONITOR\n";

    std::cout
        << "========================================\n";


    // --------------------------------------------------------
    // Initialize WMI
    // --------------------------------------------------------

    WmiClient wmi;


    if (!wmi.initialize())
    {
        std::cerr
            << "\nFailed to initialize WMI.\n";

        return 1;
    }


    // --------------------------------------------------------
    // Start monitoring engine
    // --------------------------------------------------------

    MonitorEngine monitor(wmi);

    monitor.run();


    return 0;
}