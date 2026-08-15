#include "system_monitor/collectors/gpu/GpuCollector.h"

#include <windows.h>

namespace
{
    // ========================================================
    // NVML definitions
    // ========================================================

    using nvmlReturn_t = int;

    constexpr nvmlReturn_t NVML_SUCCESS = 0;

    constexpr unsigned int NVML_TEMPERATURE_GPU = 0;

    constexpr unsigned int NVML_CLOCK_GRAPHICS = 0;
    constexpr unsigned int NVML_CLOCK_MEM = 2;


    // ========================================================
    // NVML structures
    // ========================================================

    struct nvmlDevice_t_st;

    using nvmlDevice_t = nvmlDevice_t_st*;


    struct nvmlUtilization_t
    {
        unsigned int gpu;
        unsigned int memory;
    };


    struct nvmlMemory_t
    {
        unsigned long long total;
        unsigned long long free;
        unsigned long long used;
    };


    // ========================================================
    // NVML function types
    // ========================================================

    using nvmlInit_t =
        nvmlReturn_t(*)();


    using nvmlShutdown_t =
        nvmlReturn_t(*)();


    using nvmlDeviceGetCount_t =
        nvmlReturn_t(*)(unsigned int*);


    using nvmlDeviceGetHandleByIndex_t =
        nvmlReturn_t(*)(unsigned int, nvmlDevice_t*);


    using nvmlDeviceGetName_t =
        nvmlReturn_t(*)(nvmlDevice_t, char*, unsigned int);


    using nvmlDeviceGetUtilizationRates_t =
        nvmlReturn_t(*)(nvmlDevice_t, nvmlUtilization_t*);


    using nvmlDeviceGetMemoryInfo_t =
        nvmlReturn_t(*)(nvmlDevice_t, nvmlMemory_t*);


    using nvmlDeviceGetTemperature_t =
        nvmlReturn_t(*)
        (
            nvmlDevice_t,
            unsigned int,
            unsigned int*
            );


    using nvmlDeviceGetClockInfo_t =
        nvmlReturn_t(*)
        (
            nvmlDevice_t,
            unsigned int,
            unsigned int*
            );


    // ========================================================
    // Helpers
    // ========================================================

    template <typename T>
    T getFunction(
        HMODULE module,
        const char* name)
    {
        return reinterpret_cast<T>(
            GetProcAddress(module, name)
            );
    }
}


// ============================================================
// GPU collection
// ============================================================

GpuInfo GpuCollector::collect()
{
    GpuInfo info;

    HMODULE nvml =
        LoadLibraryW(L"nvml.dll");

    if (!nvml)
    {
        return info;
    }


    // ========================================================
    // Load functions
    // ========================================================

    auto nvmlInit =
        getFunction<nvmlInit_t>(
            nvml,
            "nvmlInit_v2"
        );

    auto nvmlShutdown =
        getFunction<nvmlShutdown_t>(
            nvml,
            "nvmlShutdown"
        );

    auto nvmlDeviceGetCount =
        getFunction<nvmlDeviceGetCount_t>(
            nvml,
            "nvmlDeviceGetCount_v2"
        );

    auto nvmlDeviceGetHandleByIndex =
        getFunction<nvmlDeviceGetHandleByIndex_t>(
            nvml,
            "nvmlDeviceGetHandleByIndex_v2"
        );

    auto nvmlDeviceGetName =
        getFunction<nvmlDeviceGetName_t>(
            nvml,
            "nvmlDeviceGetName"
        );

    auto nvmlDeviceGetUtilizationRates =
        getFunction<nvmlDeviceGetUtilizationRates_t>(
            nvml,
            "nvmlDeviceGetUtilizationRates"
        );

    auto nvmlDeviceGetMemoryInfo =
        getFunction<nvmlDeviceGetMemoryInfo_t>(
            nvml,
            "nvmlDeviceGetMemoryInfo"
        );

    auto nvmlDeviceGetTemperature =
        getFunction<nvmlDeviceGetTemperature_t>(
            nvml,
            "nvmlDeviceGetTemperature"
        );

    auto nvmlDeviceGetClockInfo =
        getFunction<nvmlDeviceGetClockInfo_t>(
            nvml,
            "nvmlDeviceGetClockInfo"
        );


    // ========================================================
    // Validate functions
    // ========================================================

    if (!nvmlInit ||
        !nvmlShutdown ||
        !nvmlDeviceGetCount ||
        !nvmlDeviceGetHandleByIndex ||
        !nvmlDeviceGetName ||
        !nvmlDeviceGetUtilizationRates ||
        !nvmlDeviceGetMemoryInfo ||
        !nvmlDeviceGetTemperature ||
        !nvmlDeviceGetClockInfo)
    {
        FreeLibrary(nvml);

        return info;
    }


    // ========================================================
    // Initialize NVML
    // ========================================================

    if (nvmlInit() != NVML_SUCCESS)
    {
        FreeLibrary(nvml);

        return info;
    }


    // ========================================================
    // Get GPU count
    // ========================================================

    unsigned int deviceCount = 0;

    if (nvmlDeviceGetCount(&deviceCount)
        != NVML_SUCCESS ||
        deviceCount == 0)
    {
        nvmlShutdown();
        FreeLibrary(nvml);

        return info;
    }


    // ========================================================
    // Get first GPU
    // ========================================================

    nvmlDevice_t device = nullptr;

    if (nvmlDeviceGetHandleByIndex(
        0,
        &device)
        != NVML_SUCCESS)
    {
        nvmlShutdown();
        FreeLibrary(nvml);

        return info;
    }


    // ========================================================
    // GPU name
    // ========================================================

    char name[256] = {};

    if (nvmlDeviceGetName(
        device,
        name,
        sizeof(name))
        == NVML_SUCCESS)
    {
        info.name = name;
    }


    // ========================================================
    // GPU utilization
    // ========================================================

    nvmlUtilization_t utilization{};

    if (nvmlDeviceGetUtilizationRates(
        device,
        &utilization)
        == NVML_SUCCESS)
    {
        info.utilizationPercent =
            utilization.gpu;
    }


    // ========================================================
    // GPU memory
    // ========================================================

    nvmlMemory_t memory{};

    if (nvmlDeviceGetMemoryInfo(
        device,
        &memory)
        == NVML_SUCCESS)
    {
        info.memoryTotal = memory.total;
        info.memoryUsed = memory.used;
        info.memoryFree = memory.free;
    }


    // ========================================================
    // Temperature
    // ========================================================

    unsigned int temperature = 0;

    if (nvmlDeviceGetTemperature(
        device,
        NVML_TEMPERATURE_GPU,
        &temperature)
        == NVML_SUCCESS)
    {
        info.temperature = temperature;
    }


    // ========================================================
    // GPU core clock
    // ========================================================

    unsigned int coreClock = 0;

    if (nvmlDeviceGetClockInfo(
        device,
        NVML_CLOCK_GRAPHICS,
        &coreClock)
        == NVML_SUCCESS)
    {
        info.coreClockMHz = coreClock;
    }


    // ========================================================
    // GPU memory clock
    // ========================================================

    unsigned int memoryClock = 0;

    if (nvmlDeviceGetClockInfo(
        device,
        NVML_CLOCK_MEM,
        &memoryClock)
        == NVML_SUCCESS)
    {
        info.memoryClockMHz = memoryClock;
    }


    // ========================================================
    // Mark available
    // ========================================================

    info.available = true;


    // ========================================================
    // Cleanup
    // ========================================================

    nvmlShutdown();

    FreeLibrary(nvml);

    return info;
}