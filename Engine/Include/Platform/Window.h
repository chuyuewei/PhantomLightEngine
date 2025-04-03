/**
 * @file Window.h
 * @brief 窗口系统定义
 */

#pragma once

#include <memory>
#include <string>
#include <functional>

#include "../PhantomLightEngine.h"
#include "../Math/Vector.h"

namespace PLE {

/**
 * @brief 窗口事件类型
 */
enum class WindowEventType {
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

/**
 * @brief 窗口事件数据基类
 */
struct WindowEventData {
    WindowEventType type = WindowEventType::None;
};

/**
 * @brief 窗口调整大小事件数据
 */
struct WindowResizeEventData : public WindowEventData {
    WindowResizeEventData(unsigned int width, unsigned int height)
        : width(width), height(height) {
        type = WindowEventType::WindowResize;
    }

    unsigned int width;
    unsigned int height;
};

/**
 * @brief 键盘事件数据
 */
struct KeyEventData : public WindowEventData {
    KeyEventData(int keyCode) : keyCode(keyCode) {}

    int keyCode;
};

/**
 * @brief 鼠标事件数据
 */
struct MouseEventData : public WindowEventData {
    MouseEventData(float x, float y) : x(x), y(y) {}

    float x;
    float y;
};

/**
 * @brief 窗口事件回调函数类型
 */
using WindowEventCallbackFn = std::function<void(const WindowEventData&)>;

/**
 * @brief 窗口属性结构体
 */
struct WindowProps {
    std::string title;
    unsigned int width;
    unsigned int height;
    bool fullscreen;
    bool vsync;
    bool resizable;

    WindowProps(const std::string& title = "PhantomLight Engine",
               unsigned int width = 1280,
               unsigned int height = 720,
               bool fullscreen = false,
               bool vsync = true,
               bool resizable = true)
        : title(title), width(width), height(height),
          fullscreen(fullscreen), vsync(vsync), resizable(resizable) {}
};

/**
 * @brief 窗口接口类
 */
class PLE_API Window {
public:
    /**
     * @brief 创建窗口实例
     * @param props 窗口属性
     * @return 窗口实例
     */
    static std::shared_ptr<Window> Create(const WindowProps& props = WindowProps());

    /**
     * @brief 析构函数
     */
    virtual ~Window() = default;

    /**
     * @brief 更新窗口
     */
    virtual void Update() = 0;

    /**
     * @brief 处理窗口事件
     */
    virtual void ProcessEvents() = 0;

    /**
     * @brief 获取窗口宽度
     * @return 窗口宽度
     */
    virtual unsigned int GetWidth() const = 0;

    /**
     * @brief 获取窗口高度
     * @return 窗口高度
     */
    virtual unsigned int GetHeight() const = 0;

    /**
     * @brief 设置窗口事件回调函数
     * @param callback 回调函数
     */
    virtual void SetEventCallback(const WindowEventCallbackFn& callback) = 0;

    /**
     * @brief 设置垂直同步
     * @param enabled 是否启用
     */
    virtual void SetVSync(bool enabled) = 0;

    /**
     * @brief 是否启用垂直同步
     * @return 是否启用
     */
    virtual bool IsVSync() const = 0;

    /**
     * @brief 窗口是否应该关闭
     * @return 是否应该关闭
     */
    virtual bool ShouldClose() const = 0;

    /**
     * @brief 关闭窗口
     */
    virtual void Close() = 0;

    /**
     * @brief 获取原生窗口句柄
     * @return 原生窗口句柄
     */
    virtual void* GetNativeWindow() const = 0;

    /**
     * @brief 设置窗口标题
     * @param title 窗口标题
     */
    virtual void SetTitle(const std::string& title) = 0;

    /**
     * @brief 设置窗口大小
     * @param width 窗口宽度
     * @param height 窗口高度
     */
    virtual void SetSize(unsigned int width, unsigned int height) = 0;

    /**
     * @brief 设置窗口位置
     * @param x X坐标
     * @param y Y坐标
     */
    virtual void SetPosition(int x, int y) = 0;

    /**
     * @brief 最小化窗口
     */
    virtual void Minimize() = 0;

    /**
     * @brief 最大化窗口
     */
    virtual void Maximize() = 0;

    /**
     * @brief 恢复窗口
     */
    virtual void Restore() = 0;

    /**
     * @brief 设置窗口全屏模式
     * @param fullscreen 是否全屏
     */
    virtual void SetFullscreen(bool fullscreen) = 0;

    /**
     * @brief 窗口是否全屏
     * @return 是否全屏
     */
    virtual bool IsFullscreen() const = 0;
};

} // namespace PLE