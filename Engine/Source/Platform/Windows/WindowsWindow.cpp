/**
 * @file WindowsWindow.cpp
 * @brief Windows平台窗口实现
 */

#include "WindowsWindow.h"

#ifdef PLE_PLATFORM_WINDOWS

#include <iostream>

namespace PLE {

// 静态实例映射表，用于在WindowProc中查找窗口实例
static std::unordered_map<HWND, WindowsWindow*> s_WindowsMap;

// 实现Window::Create静态方法
std::shared_ptr<Window> Window::Create(const WindowProps& props) {
    return std::make_shared<WindowsWindow>(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
    Init(props);
}

WindowsWindow::~WindowsWindow() {
    Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;
    m_Data.vsync = props.vsync;
    m_Data.fullscreen = props.fullscreen;
    m_Data.shouldClose = false;
    m_Data.resizable = props.resizable;

    std::cout << "创建Windows窗口: " << props.title << " (" << props.width << "x" << props.height << ")" << std::endl;

    // 注册窗口类
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowsWindow::WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "PhantomLightEngineWindow";

    if (!RegisterClassEx(&wc)) {
        std::cerr << "注册窗口类失败！" << std::endl;
        return;
    }

    // 计算窗口样式
    DWORD style = WS_OVERLAPPEDWINDOW;
    if (!m_Data.resizable) {
        style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    }

    if (m_Data.fullscreen) {
        style = WS_POPUP | WS_VISIBLE;
    }

    // 调整窗口大小，考虑窗口边框
    RECT rect = { 0, 0, static_cast<LONG>(m_Data.width), static_cast<LONG>(m_Data.height) };
    AdjustWindowRect(&rect, style, FALSE);

    // 创建窗口
    m_Window = CreateWindowEx(
        0,
        "PhantomLightEngineWindow",
        m_Data.title.c_str(),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (!m_Window) {
        std::cerr << "创建窗口失败！" << std::endl;
        return;
    }

    // 获取设备上下文
    m_DeviceContext = GetDC(m_Window);

    // 保存窗口实例到映射表
    s_WindowsMap[m_Window] = this;

    // 如果是全屏模式，设置全屏
    if (m_Data.fullscreen) {
        SetFullscreen(true);
    }

    // 显示窗口
    ShowWindow(m_Window, SW_SHOW);
    UpdateWindow(m_Window);

    // 设置垂直同步
    SetVSync(m_Data.vsync);
}

void WindowsWindow::Shutdown() {
    // 从映射表中移除窗口实例
    s_WindowsMap.erase(m_Window);

    // 释放设备上下文
    if (m_DeviceContext) {
        ReleaseDC(m_Window, m_DeviceContext);
        m_DeviceContext = nullptr;
    }

    // 销毁窗口
    if (m_Window) {
        DestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

void WindowsWindow::Update() {
    // 更新窗口，可以在这里添加其他逻辑
}

void WindowsWindow::ProcessEvents() {
    // 处理Windows消息
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            m_Data.shouldClose = true;
            return;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WindowsWindow::SetVSync(bool enabled) {
    m_Data.vsync = enabled;
    // 实际的VSync设置需要在渲染API中实现
    // 例如，在OpenGL中使用wglSwapIntervalEXT，在DirectX中使用IDXGISwapChain::Present
}

void WindowsWindow::Close() {
    m_Data.shouldClose = true;
    PostMessage(m_Window, WM_CLOSE, 0, 0);
}

void WindowsWindow::SetTitle(const std::string& title) {
    m_Data.title = title;
    SetWindowText(m_Window, title.c_str());
}

void WindowsWindow::SetSize(unsigned int width, unsigned int height) {
    m_Data.width = width;
    m_Data.height = height;

    // 调整窗口大小，考虑窗口边框
    RECT rect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    DWORD style = GetWindowLong(m_Window, GWL_STYLE);
    AdjustWindowRect(&rect, style, FALSE);

    SetWindowPos(
        m_Window,
        nullptr,
        0, 0,
        rect.right - rect.left, rect.bottom - rect.top,
        SWP_NOMOVE | SWP_NOZORDER
    );
}

void WindowsWindow::SetPosition(int x, int y) {
    SetWindowPos(
        m_Window,
        nullptr,
        x, y,
        0, 0,
        SWP_NOSIZE | SWP_NOZORDER
    );
}

void WindowsWindow::Minimize() {
    ShowWindow(m_Window, SW_MINIMIZE);
}

void WindowsWindow::Maximize() {
    ShowWindow(m_Window, SW_MAXIMIZE);
}

void WindowsWindow::Restore() {
    ShowWindow(m_Window, SW_RESTORE);
}

void WindowsWindow::SetFullscreen(bool fullscreen) {
    if (m_Data.fullscreen == fullscreen) {
        return;
    }

    m_Data.fullscreen = fullscreen;

    if (fullscreen) {
        // 保存窗口位置和大小
        GetWindowRect(m_Window, &m_WindowRect);

        // 获取当前显示器信息
        HMONITOR monitor = MonitorFromWindow(m_Window, MONITOR_DEFAULTTONEAREST);
        MONITORINFO monitorInfo = { sizeof(monitorInfo) };
        GetMonitorInfo(monitor, &monitorInfo);

        // 设置全屏模式
        SetWindowLong(m_Window, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(
            m_Window,
            HWND_TOP,
            monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
            monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
            SWP_FRAMECHANGED
        );
    } else {
        // 恢复窗口样式
        DWORD style = WS_OVERLAPPEDWINDOW;
        if (!m_Data.resizable) {
            style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
        }

        SetWindowLong(m_Window, GWL_STYLE, style);

        // 恢复窗口位置和大小
        SetWindowPos(
            m_Window,
            HWND_TOP,
            m_WindowRect.left, m_WindowRect.top,
            m_WindowRect.right - m_WindowRect.left,
            m_WindowRect.bottom - m_WindowRect.top,
            SWP_FRAMECHANGED
        );
    }
}

LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // 查找窗口实例
    WindowsWindow* window = nullptr;
    if (s_WindowsMap.find(hwnd) != s_WindowsMap.end()) {
        window = s_WindowsMap[hwnd];
    }

    switch (msg) {
        case WM_CLOSE:
            if (window) {
                window->m_Data.shouldClose = true;
                if (window->m_Data.eventCallback) {
                    WindowEventData eventData;
                    eventData.type = WindowEventType::WindowClose;
                    window->m_Data.eventCallback(eventData);
                }
            }
            return 0;

        case WM_SIZE:
            if (window) {
                window->m_Data.width = LOWORD(lParam);
                window->m_Data.height = HIWORD(lParam);

                if (window->m_Data.eventCallback) {
                    WindowResizeEventData eventData(window->m_Data.width, window->m_Data.height);
                    window->m_Data.eventCallback(eventData);
                }

                // 处理最小化和最大化
                if (wParam == SIZE_MINIMIZED) {
                    // 窗口最小化
                } else if (wParam == SIZE_MAXIMIZED) {
                    // 窗口最大化
                } else if (wParam == SIZE_RESTORED) {
                    // 窗口恢复
                }
            }
            return 0;

        case WM_SETFOCUS:
            if (window && window->m_Data.eventCallback) {
                WindowEventData eventData;
                eventData.type = WindowEventType::WindowFocus;
                window->m_Data.eventCallback(eventData);
            }
            return 0;

        case WM_KILLFOCUS:
            if (window && window->m_Data.eventCallback) {
                WindowEventData eventData;
                eventData.type = WindowEventType::WindowLostFocus;
                window->m_Data.eventCallback(eventData);
            }
            return 0;

        case WM_MOVE:
            if (window && window->m_Data.eventCallback) {
                WindowEventData eventData;
                eventData.type = WindowEventType::WindowMoved;
                window->m_Data.eventCallback(eventData);
            }
            return 0;

        // 键盘事件处理
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (window && window->m_Data.eventCallback) {
                KeyEventData eventData(static_cast<int>(wParam));
                eventData.type = WindowEventType::KeyPressed;
                window->m_Data.eventCallback(eventData);
            }
            return 0;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (window && window->m_Data.eventCallback) {
                KeyEventData eventData(static_cast<int>(wParam));
                eventData.type = WindowEventType::KeyReleased;
                window->m_Data.eventCallback(eventData);
            }
            return 0;

        case WM_CHAR:
            if (window && window->m_Data.eventCallback) {
                KeyEventData eventData(static_cast<int>(wParam));
                eventData.type = WindowEventType::KeyTyped;
                window->m_Data.eventCallback(eventData);
            }
            return 0;

        // 鼠标事件处理
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            if (window && window->m_Data.eventCallback) {
                MouseEventData eventData(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                eventData.type = WindowEventType::MouseButtonPressed;
                window->m_Data.eventCallback(eventData);
            }
            return 0;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            if (window && window->m_Data.eventCallback) {
                MouseEventData eventData(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                eventData.type = WindowEventType::MouseButtonReleased;
                window->m_Data.eventCallback(eventData);
            }
            return 0;

        case WM_MOUSEMOVE:
            if (window && window->m_Data.eventCallback) {
                MouseEventData eventData(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                eventData.type = WindowEventType::MouseMoved;
                window->m_Data.eventCallback(eventData);
            }
            return 0;

        case WM_MOUSEWHEEL:
            if (window && window->m_Data.eventCallback) {
                MouseEventData eventData(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                eventData.type = WindowEventType::MouseScrolled;
                window->m_Data.eventCallback(eventData);
            }
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

} // namespace PLE

#endif // PLE_PLATFORM_WINDOWS