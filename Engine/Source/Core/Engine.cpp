/**
 * @file Engine.cpp
 * @brief 引擎核心类实现
 */

// 修改包含路径，使用更明确的路径
#include "Core/Engine.h"  // 使用项目相对路径而不是文件相对路径
#include <chrono>
#include <thread>
#include <iostream>

namespace PLE {

// 静态单例实例
static Engine* s_Instance = nullptr;

Engine& Engine::Get() {
    if (!s_Instance) {
        s_Instance = new Engine();
    }
    return *s_Instance;
}

Engine::Engine()
    : m_Running(false)
    , m_Initialized(false)
    , m_FPS(0.0f)
    , m_DeltaTime(0.0f)
    , m_Window(nullptr)
    , m_RenderSystem(nullptr)
    , m_PhysicsSystem(nullptr)
    , m_ResourceManager(nullptr)
    , m_PluginManager(nullptr)
    , m_ActiveScene(nullptr) {
}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize(const EngineConfig& config) {
    if (m_Initialized) {
        std::cerr << "引擎已经初始化！" << std::endl;
        return false;
    }

    m_Config = config;
    
    // 创建窗口（这里需要实现Window类）
    // m_Window = Window::Create(config.applicationName, config.windowWidth, config.windowHeight, config.fullscreen);
    // if (!m_Window) {
    //     std::cerr << "创建窗口失败！" << std::endl;
    //     return false;
    // }

    // 初始化渲染系统
    RenderSystemConfig renderConfig;
    renderConfig.enableVSync = config.vsync;
    renderConfig.enableDebugMode = config.enableValidation;
    
    m_RenderSystem = RenderSystem::Create(renderConfig);
    if (!m_RenderSystem) {
        std::cerr << "创建渲染系统失败！" << std::endl;
        return false;
    }
    
    // 初始化渲染系统（需要Window类）
    // if (!m_RenderSystem->Initialize(m_Window)) {
    //     std::cerr << "初始化渲染系统失败！" << std::endl;
    //     return false;
    // }

    // 初始化物理系统
    // m_PhysicsSystem = PhysicsSystem::Create();
    // if (!m_PhysicsSystem) {
    //     std::cerr << "创建物理系统失败！" << std::endl;
    //     return false;
    // }

    // 初始化资源管理器
    // m_ResourceManager = std::make_shared<ResourceManager>();
    // if (!m_ResourceManager) {
    //     std::cerr << "创建资源管理器失败！" << std::endl;
    //     return false;
    // }

    // 初始化插件系统
    // m_PluginManager = std::make_shared<PluginManager>();
    // if (!m_PluginManager) {
    //     std::cerr << "创建插件管理器失败！" << std::endl;
    //     return false;
    // }

    // 创建默认场景
    m_ActiveScene = CreateScene("Default Scene");
    if (!m_ActiveScene) {
        std::cerr << "创建默认场景失败！" << std::endl;
        return false;
    }

    m_Initialized = true;
    std::cout << "引擎初始化成功！" << std::endl;
    return true;
}

void Engine::Shutdown() {
    if (!m_Initialized) {
        return;
    }

    m_Running = false;

    // 清理场景
    m_Scenes.clear();
    m_ActiveScene = nullptr;

    // 关闭插件系统
    if (m_PluginManager) {
        // m_PluginManager->Shutdown();
        m_PluginManager = nullptr;
    }

    // 关闭资源管理器
    if (m_ResourceManager) {
        // m_ResourceManager->Shutdown();
        m_ResourceManager = nullptr;
    }

    // 关闭物理系统
    if (m_PhysicsSystem) {
        // m_PhysicsSystem->Shutdown();
        m_PhysicsSystem = nullptr;
    }

    // 关闭渲染系统
    if (m_RenderSystem) {
        m_RenderSystem->Shutdown();
        m_RenderSystem = nullptr;
    }

    // 关闭窗口
    if (m_Window) {
        // m_Window->Close();
        m_Window = nullptr;
    }

    m_Initialized = false;
    std::cout << "引擎已关闭！" << std::endl;
}

void Engine::Run() {
    if (!m_Initialized) {
        std::cerr << "引擎未初始化！" << std::endl;
        return;
    }

    m_Running = true;

    // 主循环
    auto lastTime = std::chrono::high_resolution_clock::now();
    while (m_Running) {
        // 计算帧时间
        auto currentTime = std::chrono::high_resolution_clock::now();
        m_DeltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // 计算帧率
        CalculateFrameStats();

        // 处理窗口事件
        // if (m_Window) {
        //     m_Window->ProcessEvents();
        //     if (m_Window->ShouldClose()) {
        //         m_Running = false;
        //         continue;
        //     }
        // }

        // 更新引擎状态
        Update();

        // 渲染场景
        Render();

        // 限制帧率（可选）
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Engine::Update() {
    // 更新插件系统
    if (m_PluginManager) {
        // m_PluginManager->Update(m_DeltaTime);
    }

    // 更新物理系统
    if (m_PhysicsSystem) {
        // m_PhysicsSystem->Update(m_DeltaTime);
    }

    // 更新当前场景
    if (m_ActiveScene) {
        m_ActiveScene->Update(m_DeltaTime);
    }
}

void Engine::Render() {
    if (!m_RenderSystem) {
        return;
    }

    // 开始渲染
    m_RenderSystem->BeginFrame();

    // 清除缓冲区
    m_RenderSystem->Clear(Vector4(0.1f, 0.1f, 0.1f, 1.0f));

    // 渲染当前场景
    if (m_ActiveScene) {
        m_ActiveScene->Render();
    }

    // 结束渲染
    m_RenderSystem->EndFrame();
}

void Engine::CalculateFrameStats() {
    static int frameCount = 0;
    static float timeElapsed = 0.0f;

    frameCount++;
    timeElapsed += m_DeltaTime;

    if (timeElapsed >= 1.0f) {
        m_FPS = static_cast<float>(frameCount) / timeElapsed;

        // 重置计数器
        frameCount = 0;
        timeElapsed = 0.0f;

        // 输出帧率信息（调试用）
        std::cout << "FPS: " << m_FPS << ", Frame Time: " << (1000.0f / m_FPS) << "ms" << std::endl;
    }
}

std::shared_ptr<Scene> Engine::CreateScene(const std::string& name) {
    // 检查是否已存在同名场景
    auto it = m_Scenes.find(name);
    if (it != m_Scenes.end()) {
        std::cerr << "场景 '" << name << "' 已存在！" << std::endl;
        return it->second;
    }

    // 创建新场景
    auto scene = Scene::Create(name);
    if (scene) {
        m_Scenes[name] = scene;
    }

    return scene;
}

} // namespace PLE