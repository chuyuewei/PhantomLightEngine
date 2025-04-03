/**
 * @file Engine.h
 * @brief 引擎核心类定义
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../PhantomLightEngine.h"
#include "../Renderer/RenderSystem.h"
#include "../Physics/PhysicsSystem.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "PluginSystem.h"

namespace PLE {

// 前向声明
class Window;

/**
 * @brief 引擎核心类
 * 
 * Engine类是整个引擎的核心，负责初始化和管理所有子系统，
 * 包括渲染系统、物理系统、资源管理、场景管理和插件系统等。
 */
class PLE_API Engine {
public:
    /**
     * @brief 获取引擎单例实例
     * @return 引擎实例引用
     */
    static Engine& Get();

    /**
     * @brief 初始化引擎
     * @param config 引擎配置
     * @return 是否成功初始化
     */
    bool Initialize(const EngineConfig& config = EngineConfig());

    /**
     * @brief 关闭引擎
     */
    void Shutdown();

    /**
     * @brief 运行引擎主循环
     */
    void Run();

    /**
     * @brief 获取渲染系统
     * @return 渲染系统指针
     */
    std::shared_ptr<RenderSystem> GetRenderSystem() const { return m_RenderSystem; }

    /**
     * @brief 获取物理系统
     * @return 物理系统指针
     */
    std::shared_ptr<PhysicsSystem> GetPhysicsSystem() const { return m_PhysicsSystem; }

    /**
     * @brief 获取资源管理器
     * @return 资源管理器指针
     */
    std::shared_ptr<ResourceManager> GetResourceManager() const { return m_ResourceManager; }

    /**
     * @brief 获取插件管理器
     * @return 插件管理器指针
     */
    std::shared_ptr<PluginManager> GetPluginManager() const { return m_PluginManager; }

    /**
     * @brief 获取当前活动场景
     * @return 场景指针
     */
    std::shared_ptr<Scene> GetActiveScene() const { return m_ActiveScene; }

    /**
     * @brief 设置当前活动场景
     * @param scene 场景指针
     */
    void SetActiveScene(std::shared_ptr<Scene> scene) { m_ActiveScene = scene; }

    /**
     * @brief 创建新场景
     * @param name 场景名称
     * @return 场景指针
     */
    std::shared_ptr<Scene> CreateScene(const std::string& name = "New Scene");

    /**
     * @brief 获取引擎是否正在运行
     * @return 是否正在运行
     */
    bool IsRunning() const { return m_Running; }

    /**
     * @brief 停止引擎运行
     */
    void Stop() { m_Running = false; }

    /**
     * @brief 获取帧率
     * @return 当前帧率
     */
    float GetFPS() const { return m_FPS; }

    /**
     * @brief 获取帧时间
     * @return 当前帧时间（秒）
     */
    float GetDeltaTime() const { return m_DeltaTime; }

private:
    Engine(); // 私有构造函数，确保单例模式
    ~Engine();
    Engine(const Engine&) = delete; // 禁止拷贝
    Engine& operator=(const Engine&) = delete; // 禁止赋值

    /**
     * @brief 更新引擎状态
     */
    void Update();

    /**
     * @brief 渲染场景
     */
    void Render();

    /**
     * @brief 计算帧率和帧时间
     */
    void CalculateFrameStats();

private:
    bool m_Running = false;
    bool m_Initialized = false;
    float m_FPS = 0.0f;
    float m_DeltaTime = 0.0f;
    EngineConfig m_Config;

    std::shared_ptr<Window> m_Window;
    std::shared_ptr<RenderSystem> m_RenderSystem;
    std::shared_ptr<PhysicsSystem> m_PhysicsSystem;
    std::shared_ptr<ResourceManager> m_ResourceManager;
    std::shared_ptr<PluginManager> m_PluginManager;
    std::shared_ptr<Scene> m_ActiveScene;

    // 场景管理
    std::unordered_map<std::string, std::shared_ptr<Scene>> m_Scenes;
};

} // namespace PLE