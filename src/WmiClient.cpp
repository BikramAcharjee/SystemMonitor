#include "system_monitor/WmiClient.h"

#include <comdef.h>
#include <iostream>

#pragma comment(lib, "wbemuuid.lib")

WmiClient::WmiClient()
    : services_(nullptr),
    initialized_(false)
{
}

WmiClient::~WmiClient()
{
    if (services_ != nullptr)
    {
        services_->Release();
        services_ = nullptr;
    }

    if (initialized_)
    {
        CoUninitialize();
    }
}

bool WmiClient::initialize()
{
    HRESULT hr;

    // --------------------------------------------------------
    // Initialize COM
    // --------------------------------------------------------

    hr = CoInitializeEx(
        nullptr,
        COINIT_MULTITHREADED
    );

    if (FAILED(hr))
    {
        std::cerr << "Failed to initialize COM.\n";
        return false;
    }

    initialized_ = true;


    // --------------------------------------------------------
    // Initialize COM security
    // --------------------------------------------------------

    hr = CoInitializeSecurity(
        nullptr,
        -1,
        nullptr,
        nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE,
        nullptr
    );

    if (FAILED(hr) && hr != RPC_E_TOO_LATE)
    {
        std::cerr << "Failed to initialize COM security.\n";

        CoUninitialize();
        initialized_ = false;

        return false;
    }


    // --------------------------------------------------------
    // Create WMI locator
    // --------------------------------------------------------

    IWbemLocator* locator = nullptr;

    hr = CoCreateInstance(
        CLSID_WbemLocator,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        reinterpret_cast<void**>(&locator)
    );

    if (FAILED(hr))
    {
        std::cerr << "Failed to create WMI locator.\n";

        CoUninitialize();
        initialized_ = false;

        return false;
    }


    // --------------------------------------------------------
    // Connect to WMI
    // --------------------------------------------------------

    hr = locator->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        nullptr,
        nullptr,
        nullptr,
        0,
        nullptr,
        nullptr,
        &services_
    );

    locator->Release();

    if (FAILED(hr))
    {
        std::cerr << "Failed to connect to WMI.\n";

        CoUninitialize();
        initialized_ = false;

        return false;
    }


    // --------------------------------------------------------
    // Set WMI proxy security
    // --------------------------------------------------------

    hr = CoSetProxyBlanket(
        services_,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        nullptr,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE
    );

    if (FAILED(hr))
    {
        std::cerr << "Failed to configure WMI proxy security.\n";

        services_->Release();
        services_ = nullptr;

        CoUninitialize();
        initialized_ = false;

        return false;
    }

    return true;
}


// ------------------------------------------------------------
// Get a single property from a WMI query
// ------------------------------------------------------------

std::string WmiClient::getProperty(
    const std::wstring& query,
    const std::wstring& property
)
{
    if (services_ == nullptr)
    {
        return "Unknown";
    }

    IEnumWbemClassObject* enumerator = nullptr;

    HRESULT hr = services_->ExecQuery(
        _bstr_t(L"WQL"),
        _bstr_t(query.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr,
        &enumerator
    );

    if (FAILED(hr))
    {
        return "Unknown";
    }


    IWbemClassObject* object = nullptr;
    ULONG returned = 0;

    hr = enumerator->Next(
        WBEM_INFINITE,
        1,
        &object,
        &returned
    );

    if (FAILED(hr) || returned == 0)
    {
        enumerator->Release();
        return "Unknown";
    }


    VARIANT value;

    VariantInit(&value);

    hr = object->Get(
        property.c_str(),
        0,
        &value,
        nullptr,
        nullptr
    );


    std::string result = "Unknown";

    if (SUCCEEDED(hr))
    {
        if (value.vt == VT_BSTR && value.bstrVal != nullptr)
        {
            result = wideToUtf8(value.bstrVal);
        }
        else if (value.vt == VT_UI4)
        {
            result = std::to_string(value.uintVal);
        }
        else if (value.vt == VT_I4)
        {
            result = std::to_string(value.intVal);
        }
        else if (value.vt == VT_UI8)
        {
            result = std::to_string(value.ullVal);
        }
        else if (value.vt == VT_I8)
        {
            result = std::to_string(value.llVal);
        }
    }


    VariantClear(&value);

    object->Release();
    enumerator->Release();

    return result;
}


// ------------------------------------------------------------
// Convert UTF-16 Windows string → UTF-8
// ------------------------------------------------------------

std::string WmiClient::wideToUtf8(
    const std::wstring& value
)
{
    if (value.empty())
    {
        return {};
    }

    int size = WideCharToMultiByte(
        CP_UTF8,
        0,
        value.c_str(),
        static_cast<int>(value.size()),
        nullptr,
        0,
        nullptr,
        nullptr
    );

    if (size <= 0)
    {
        return {};
    }

    std::string result(size, '\0');

    WideCharToMultiByte(
        CP_UTF8,
        0,
        value.c_str(),
        static_cast<int>(value.size()),
        result.data(),
        size,
        nullptr,
        nullptr
    );

    return result;
}