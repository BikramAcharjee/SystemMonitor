#include "TimeUtils.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string getCurrentTimestamp()
{
    const auto now =
        std::chrono::system_clock::now();

    const std::time_t time =
        std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

    localtime_s(
        &localTime,
        &time
    );

    std::ostringstream output;

    output
        << std::put_time(
            &localTime,
            "%Y-%m-%dT%H:%M:%S"
        );

    return output.str();
}