#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>

namespace ApiResponse
{
    inline void json(
        httplib::Response& res,
        const nlohmann::json& data)
    {
        res.set_content(
            data.dump(4),
            "application/json"
        );
    }


    inline void error(
        httplib::Response& res,
        int status,
        const std::string& message)
    {
        nlohmann::json response =
        {
            {"error", message}
        };

        res.status = status;

        res.set_content(
            response.dump(4),
            "application/json"
        );
    }
}