/**
 * @file WindowsWindow.h
 * @brief Windows平台窗口实现
 */

#pragma once

#include "../../../Include/Platform/Window.h"

#ifdef PLE_PLATFORM_WINDOWS

#include <Windows.h>

namespace PLE {

/**
 * @brief Windows平台窗口实现
 */
class WindowsWindow : public Window {
public:
    /**
     * @brief 构造函数
     * @param props 窗口属性
     */
    WindowsWindow(const WindowProps& props);

    /**
     * @brief 析构函数
     */
    virtual ~WindowsWindow();

    /**
     * @brief 更新窗口
     */
    virtual void Update() override;

    /**
     * @brief 处理窗口事件
     */
    virtual void ProcessEvents() override;

    /**
     * @brief 获取窗口宽度
     * @return 窗口宽度
     */
    virtual unsigned int GetWidth() const override { return m_Data.width; }

    /**
     * @brief 获取窗口高度
     * @return 窗口高度
     */
    virtual unsigned int GetHeight() const override { return m_Data.height; }

    /**
     * @brief 设置窗口事件回调函数
     * @param callback 回调函数
     */
    virtual void SetEventCallback(const WindowEventCallbackFn& callback) override { m_Data.eventCallback = callback; }

    /**
     * @brief 设置垂直同步
     * @param enabled 是否启用
     */
    virtual void SetVSync(bool enabled) override;

    /**
     * @brief 是否启用垂直同步
     * @return 是否启用
     */
    virtual bool IsVSync() const override { return m_Data.vsync; }

    /**
     * @brief 窗口是否应该关闭
     * @return 是否应该关闭
     */
    virtual bool ShouldClose() const override { return m_Data.shouldClose; }

    /**
     * @brief 关闭窗口
     */
    virtual void Close() override;

    /**
     * @brief 获取原生窗口句柄
     * @return 原生窗口句柄
     */
    virtual void* GetNativeWindow() const override { return (void*)m_Window; }

    /**
     * @brief 设置窗口标题
     * @param title 窗口标题
     */
    virtual void SetTitle(const std::string& title) override;

    /**
     * @brief 设置窗口大小
     * @param width 窗口宽度
     * @param height 窗口高度
     */
    virtual void SetSize(unsigned int width, unsigned int height) override;

    /**
     * @brief 设置窗口位置
     * @param x X坐标
     * @param y Y坐标
     */
    virtual void SetPosition(int x, int y) override;

    /**
     * @brief 最小化窗口
     */
    virtual void Minimize() override;

    /**
     * @brief 最大化窗口
     */
    virtual void Maximize() override;

    /**
     * @brief 恢复窗口
     */
    virtual void Restore() override;

    /**
     * @brief 设置窗口全屏模式
     * @param fullscreen 是否全屏
     */
    virtual void SetFullscreen(bool fullscreen) override;

    /**
     * @brief 窗口是否全屏
     * @return 是否全屏
     */
    virtual bool IsFullscreen() const override { return m_Data.fullscreen; }

private:
    /**
     * @brief 初始化窗口
     */
    void Init(const WindowProps& props);

    /**
     * @brief 关闭窗口
     */
    void Shutdown();

    /**
     * @brief Windows消息处理回调函数
     */
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_Window;
    HDC m_DeviceContext;

    // 窗口数据结构，用于在回调函数中访问窗口实例
    struct WindowData {
        std::string title;
        unsigned int width, height;
        bool vsync;
        bool fullscreen;
        bool shouldClose;
        bool resizable;
        WindowEventCallbackFn eventCallback;
    };

    WindowData m_Data;
    RECT m_WindowRect; // 用于保存窗口位置和大小，在全屏切换时使用
};

} // namespace PLE

#endif // PLE_PLATFORM_WINDOWS