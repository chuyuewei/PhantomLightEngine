/**
 * @file ResourceManager.h
 * @brief 资源管理系统定义
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <future>

#include "../PhantomLightEngine.h"

namespace PLE {

// 前向声明
class Resource;
class Texture;
class Mesh;
class Shader;
class Material;
class AudioClip;

/**
 * @brief 资源基类
 */
class PLE_API Resource {
public:
    /**
     * @brief 构造函数
     * @param path 资源路径
     */
    Resource(const std::string& path);

    /**
     * @brief 析构函数
     */
    virtual ~Resource() = default;

    /**
     * @brief 获取资源路径
     * @return 资源路径
     */
    const std::string& GetPath() const { return m_Path; }

    /**
     * @brief 获取资源名称
     * @return 资源名称
     */
    const std::string& GetName() const { return m_Name; }

    /**
     * @brief 资源是否已加载
     * @return 是否已加载
     */
    bool IsLoaded() const { return m_IsLoaded; }

    /**
     * @brief 加载资源
     * @return 是否成功加载
     */
    virtual bool Load() = 0;

    /**
     * @brief 卸载资源
     */
    virtual void Unload() = 0;

protected:
    std::string m_Path;
    std::string m_Name;
    bool m_IsLoaded;
};

/**
 * @brief 资源管理器类
 */
class PLE_API ResourceManager {
public:
    /**
     * @brief 获取资源管理器实例
     * @return 资源管理器实例
     */
    static ResourceManager& GetInstance();

    /**
     * @brief 初始化资源管理器
     * @param basePath 资源基础路径
     * @return 是否成功初始化
     */
    bool Initialize(const std::string& basePath = "");

    /**
     * @brief 关闭资源管理器
     */
    void Shutdown();

    /**
     * @brief 设置资源基础路径
     * @param basePath 资源基础路径
     */
    void SetBasePath(const std::string& basePath) { m_BasePath = basePath; }

    /**
     * @brief 获取资源基础路径
     * @return 资源基础路径
     */
    const std::string& GetBasePath() const { return m_BasePath; }

    /**
     * @brief 加载资源
     * @tparam T 资源类型
     * @param path 资源路径
     * @param immediate 是否立即加载
     * @return 资源指针
     */
    template<typename T>
    std::shared_ptr<T> Load(const std::string& path, bool immediate = true);

    /**
     * @brief 异步加载资源
     * @tparam T 资源类型
     * @param path 资源路径
     * @return 资源加载的future
     */
    template<typename T>
    std::future<std::shared_ptr<T>> LoadAsync(const std::string& path);

    /**
     * @brief 获取资源
     * @tparam T 资源类型
     * @param path 资源路径
     * @return 资源指针，如果未找到则返回nullptr
     */
    template<typename T>
    std::shared_ptr<T> Get(const std::string& path);

    /**
     * @brief 卸载资源
     * @tparam T 资源类型
     * @param path 资源路径
     * @return 是否成功卸载
     */
    template<typename T>
    bool Unload(const std::string& path);

    /**
     * @brief 卸载所有资源
     */
    void UnloadAll();

    /**
     * @brief 注册资源工厂
     * @tparam T 资源类型
     * @param factory 资源工厂函数
     */
    template<typename T>
    void RegisterResourceFactory(std::function<std::shared_ptr<T>(const std::string&)> factory);

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::string m_BasePath;
    std::unordered_map<std::type_index, std::function<std::shared_ptr<Resource>(const std::string&)>> m_ResourceFactories;
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_Resources;
};

// 模板方法实现
template<typename T>
std::shared_ptr<T> ResourceManager::Load(const std::string& path, bool immediate) {
    static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

    // 检查资源是否已加载
    std::string fullPath = m_BasePath + path;
    auto it = m_Resources.find(fullPath);
    if (it != m_Resources.end()) {
        return std::dynamic_pointer_cast<T>(it->second);
    }

    // 查找资源工厂
    auto factoryIt = m_ResourceFactories.find(typeid(T));
    if (factoryIt == m_ResourceFactories.end()) {
        return nullptr;
    }

    // 创建资源
    std::shared_ptr<Resource> resource = factoryIt->second(fullPath);
    if (!resource) {
        return nullptr;
    }

    // 加载资源
    if (immediate) {
        if (!resource->Load()) {
            return nullptr;
        }
    }

    // 存储资源
    m_Resources[fullPath] = resource;
    return std::dynamic_pointer_cast<T>(resource);
}

template<typename T>
std::future<std::shared_ptr<T>> ResourceManager::LoadAsync(const std::string& path) {
    static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

    return std::async(std::launch::async, [this, path]() {
        std::shared_ptr<T> resource = Load<T>(path, false);
        if (resource) {
            resource->Load();
        }
        return resource;
    });
}

template<typename T>
std::shared_ptr<T> ResourceManager::Get(const std::string& path) {
    static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

    std::string fullPath = m_BasePath + path;
    auto it = m_Resources.find(fullPath);
    if (it != m_Resources.end()) {
        return std::dynamic_pointer_cast<T>(it->second);
    }
    return nullptr;
}

template<typename T>
bool ResourceManager::Unload(const std::string& path) {
    static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

    std::string fullPath = m_BasePath + path;
    auto it = m_Resources.find(fullPath);
    if (it != m_Resources.end()) {
        it->second->Unload();
        m_Resources.erase(it);
        return true;
    }
    return false;
}

template<typename T>
void ResourceManager::RegisterResourceFactory(std::function<std::shared_ptr<T>(const std::string&)> factory) {
    static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

    m_ResourceFactories[typeid(T)] = [factory](const std::string& path) {
        return factory(path);
    };
}

} // namespace PLE