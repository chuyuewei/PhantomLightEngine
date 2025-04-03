/**
 * @file PluginSystem.h
 * @brief 插件系统定义
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "../PhantomLightEngine.h"

namespace PLE {

// 前向声明
class Plugin;
class PluginManager;

/**
 * @brief 插件接口
 */
class PLE_API Plugin {
public:
    /**
     * @brief 构造函数
     * @param name 插件名称
     * @param version 插件版本
     */
    Plugin(const std::string& name, const std::string& version);

    /**
     * @brief 析构函数
     */
    virtual ~Plugin() = default;

    /**
     * @brief 获取插件名称
     * @return 插件名称
     */
    const std::string& GetName() const { return m_Name; }

    /**
     * @brief 获取插件版本
     * @return 插件版本
     */
    const std::string& GetVersion() const { return m_Version; }

    /**
     * @brief 初始化插件
     * @return 是否成功初始化
     */
    virtual bool Initialize() = 0;

    /**
     * @brief 关闭插件
     */
    virtual void Shutdown() = 0;

    /**
     * @brief 更新插件
     * @param deltaTime 帧时间间隔（秒）
     */
    virtual void Update(float deltaTime) = 0;

private:
    std::string m_Name;
    std::string m_Version;
};

/**
 * @brief 插件创建函数类型
 */
using PluginCreateFunc = Plugin* (*)();

/**
 * @brief 插件销毁函数类型
 */
using PluginDestroyFunc = void (*)(Plugin*);

/**
 * @brief 插件信息结构体
 */
struct PluginInfo {
    std::string name;
    std::string version;
    std::string description;
    std::string author;
    std::string url;
    std::vector<std::string> dependencies;
};

/**
 * @brief 插件管理器类
 */
class PLE_API PluginManager {
public:
    /**
     * @brief 获取插件管理器实例
     * @return 插件管理器实例
     */
    static PluginManager& GetInstance();

    /**
     * @brief 初始化插件管理器
     * @param pluginDir 插件目录路径
     * @return 是否成功初始化
     */
    bool Initialize(const std::string& pluginDir = "Plugins");

    /**
     * @brief 关闭插件管理器
     */
    void Shutdown();

    /**
     * @brief 更新所有插件
     * @param deltaTime 帧时间间隔（秒）
     */
    void UpdateAll(float deltaTime);

    /**
     * @brief 加载插件
     * @param pluginPath 插件路径
     * @return 是否成功加载
     */
    bool LoadPlugin(const std::string& pluginPath);

    /**
     * @brief 卸载插件
     * @param pluginName 插件名称
     * @return 是否成功卸载
     */
    bool UnloadPlugin(const std::string& pluginName);

    /**
     * @brief 获取插件
     * @param pluginName 插件名称
     * @return 插件指针，如果未找到则返回nullptr
     */
    Plugin* GetPlugin(const std::string& pluginName) const;

    /**
     * @brief 获取所有插件
     * @return 插件映射表
     */
    const std::unordered_map<std::string, Plugin*>& GetAllPlugins() const { return m_Plugins; }

    /**
     * @brief 获取插件信息
     * @param pluginPath 插件路径
     * @return 插件信息
     */
    PluginInfo GetPluginInfo(const std::string& pluginPath) const;

    /**
     * @brief 注册静态插件
     * @tparam T 插件类型
     * @return 是否成功注册
     */
    template<typename T>
    bool RegisterStaticPlugin();

private:
    PluginManager() = default;
    ~PluginManager() = default;
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    struct PluginModule {
        void* handle;
        Plugin* instance;
        PluginCreateFunc createFunc;
        PluginDestroyFunc destroyFunc;
    };

    std::string m_PluginDir;
    std::unordered_map<std::string, PluginModule> m_PluginModules;
    std::unordered_map<std::string, Plugin*> m_Plugins;

    bool LoadPluginModule(const std::string& pluginPath, PluginModule& module);
    void UnloadPluginModule(PluginModule& module);
};

// 模板方法实现
template<typename T>
bool PluginManager::RegisterStaticPlugin() {
    static_assert(std::is_base_of<Plugin, T>::value, "T must derive from Plugin");

    T* plugin = new