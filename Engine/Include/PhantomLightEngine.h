/**
 * @file PhantomLightEngine.h
 * @brief 幻光引擎核心头文件
 */

#pragma once

// 版本信息
#define PLE_VERSION_MAJOR 0
#define PLE_VERSION_MINOR 1
#define PLE_VERSION_PATCH 0

// 平台检测
#if defined(_WIN32) || defined(_WIN64)
    #define PLE_PLATFORM_WINDOWS
#elif defined(__APPLE__)
    #define PLE_PLATFORM_MACOS
#elif defined(__linux__)
    #define PLE_PLATFORM_LINUX
#else
    #error "不支持的平台"
#endif

// 编译器检测
#if defined(_MSC_VER)
    #define PLE_COMPILER_MSVC
#elif defined(__clang__)
    #define PLE_COMPILER_CLANG
#elif defined(__GNUC__)
    #define PLE_COMPILER_GCC
#else
    #error "不支持的编译器"
#endif

// API导出宏
#ifdef PLE_PLATFORM_WINDOWS
    #ifdef PLE_BUILD_DLL
        #define PLE_API __declspec(dllexport)
    #else
        #define PLE_API __declspec(dllimport)
    #endif
#else
    #define PLE_API __attribute__((visibility("default")))
#endif

// 调试宏
#ifdef NDEBUG
    #define PLE_RELEASE
#else
    #define PLE_DEBUG
#endif

// 断言宏
#ifdef PLE_DEBUG
    #include <cassert>
    #define PLE_ASSERT(x, ...) assert(x)
#else
    #define PLE_ASSERT(x, ...)
#endif

// 命名空间
namespace PLE {

// 引擎初始化配置
struct EngineConfig {
    const char* applicationName = "PhantomLightEngine Application";
    int windowWidth = 1280;
    int windowHeight = 720;
    bool fullscreen = false;
    bool vsync = true;
    bool enableValidation = true;
};

// 引擎初始化函数
bool Initialize(const EngineConfig& config = EngineConfig());

// 引擎关闭函数
void Shutdown();

} // namespace PLE