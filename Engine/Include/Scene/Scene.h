/**
 * @file Scene.h
 * @brief 场景系统定义
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <functional>

#include "../PhantomLightEngine.h"
#include "../Math/Vector.h"
#include "../Math/Matrix4.h"
#include "../Math/Quaternion.h"

namespace PLE {

// 前向声明
class Entity;
class Component;
class Transform;
class Camera;
class Light;
class Renderer;

/**
 * @brief 场景类
 */
class PLE_API Scene {
public:
    /**
     * @brief 创建场景实例
     * @param name 场景名称
     * @return 场景实例
     */
    static std::shared_ptr<Scene> Create(const std::string& name = "New Scene");

    /**
     * @brief 构造函数
     * @param name 场景名称
     */
    Scene(const std::string& name = "New Scene");

    /**
     * @brief 析构函数
     */
    virtual ~Scene();

    /**
     * @brief 获取场景名称
     * @return 场景名称
     */
    const std::string& GetName() const { return m_Name; }

    /**
     * @brief 设置场景名称
     * @param name 场景名称
     */
    void SetName(const std::string& name) { m_Name = name; }

    /**
     * @brief 创建实体
     * @param name 实体名称
     * @return 实体指针
     */
    std::shared_ptr<Entity> CreateEntity(const std::string& name = "Entity");

    /**
     * @brief 销毁实体
     * @param entity 实体指针
     */
    void DestroyEntity(std::shared_ptr<Entity> entity);

    /**
     * @brief 获取所有实体
     * @return 实体列表
     */
    const std::vector<std::shared_ptr<Entity>>& GetEntities() const { return m_Entities; }

    /**
     * @brief 根据名称查找实体
     * @param name 实体名称
     * @return 实体指针，如果未找到则返回nullptr
     */
    std::shared_ptr<Entity> FindEntityByName(const std::string& name);

    /**
     * @brief 更新场景
     * @param deltaTime 帧时间间隔（秒）
     */
    void Update(float deltaTime);

    /**
     * @brief 渲染场景
     */
    void Render();

    /**
     * @brief 获取主相机
     * @return 主相机指针
     */
    std::shared_ptr<Camera> GetMainCamera() const { return m_MainCamera; }

    /**
     * @brief 设置主相机
     * @param camera 相机指针
     */
    void SetMainCamera(std::shared_ptr<Camera> camera) { m_MainCamera = camera; }

private:
    std::string m_Name;
    std::vector<std::shared_ptr<Entity>> m_Entities;
    std::shared_ptr<Camera> m_MainCamera;
};

/**
 * @brief 实体类
 */
class PLE_API Entity : public std::enable_shared_from_this<Entity> {
public:
    /**
     * @brief 构造函数
     * @param scene 所属场景
     * @param name 实体名称
     */
    Entity(Scene* scene, const std::string& name = "Entity");

    /**
     * @brief 析构函数
     */
    virtual ~Entity();

    /**
     * @brief 获取实体名称
     * @return 实体名称
     */
    const std::string& GetName() const { return m_Name; }

    /**
     * @brief 设置实体名称
     * @param name 实体名称
     */
    void SetName(const std::string& name) { m_Name = name; }

    /**
     * @brief 获取实体ID
     * @return 实体ID
     */
    uint64_t GetID() const { return m_ID; }

    /**
     * @brief 获取所属场景
     * @return 场景指针
     */
    Scene* GetScene() const { return m_Scene; }

    /**
     * @brief 获取变换组件
     * @return 变换组件指针
     */
    std::shared_ptr<Transform> GetTransform() const { return m_Transform; }

    /**
     * @brief 添加组件
     * @tparam T 组件类型
     * @param args 组件构造参数
     * @return 组件指针
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args);

    /**
     * @brief 获取组件
     * @tparam T 组件类型
     * @return 组件指针，如果未找到则返回nullptr
     */
    template<typename T>
    std::shared_ptr<T> GetComponent() const;

    /**
     * @brief 获取所有组件
     * @return 组件列表
     */
    const std::vector<std::shared_ptr<Component>>& GetComponents() const { return m_Components; }

    /**
     * @brief 移除组件
     * @tparam T 组件类型
     * @return 是否成功移除
     */
    template<typename T>
    bool RemoveComponent();

    /**
     * @brief 更新实体
     * @param deltaTime 帧时间间隔（秒）
     */
    void Update(float deltaTime);

private:
    uint64_t m_ID;
    std::string m_Name;
    Scene* m_Scene;
    std::shared_ptr<Transform> m_Transform;
    std::vector<std::shared_ptr<Component>> m_Components;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> m_ComponentMap;

    static uint64_t s_NextEntityID;
};

/**
 * @brief 组件基类
 */
class PLE_API Component {
public:
    /**
     * @brief 构造函数
     * @param entity 所属实体
     */
    Component(std::shared_ptr<Entity> entity);

    /**
     * @brief 析构函数
     */
    virtual ~Component() = default;

    /**
     * @brief 获取所属实体
     * @return 实体指针
     */
    std::shared_ptr<Entity> GetEntity() const { return m_Entity.lock(); }

    /**
     * @brief 初始化组件
     */
    virtual void Initialize() {}

    /**
     * @brief 更新组件
     * @param deltaTime 帧时间间隔（秒）
     */
    virtual void Update(float deltaTime) {}

    /**
     * @brief 销毁组件
     */
    virtual void Destroy() {}

private:
    std::weak_ptr<Entity> m_Entity;
};

/**
 * @brief 变换组件
 */
class PLE_API Transform : public Component {
public:
    /**
     * @brief 构造函数
     * @param entity 所属实体
     */
    Transform(std::shared_ptr<Entity> entity);

    /**
     * @brief 获取局部位置
     * @return 局部位置
     */
    const Vector3& GetLocalPosition() const { return m_LocalPosition; }

    /**
     * @brief 设置局部位置
     * @param position 局部位置
     */
    void SetLocalPosition(const Vector3& position);

    /**
     * @brief 获取局部旋转
     * @return 局部旋转（四元数）
     */
    const Quaternion& GetLocalRotation() const { return m_LocalRotation; }

    /**
     * @brief 设置局部旋转
     * @param rotation 局部旋转（四元数）
     */
    void SetLocalRotation(const Quaternion& rotation);

    /**
     * @brief 获取局部缩放
     * @return 局部缩放
     */
    const Vector3& GetLocalScale() const { return m_LocalScale; }

    /**
     * @brief 设置局部缩放
     * @param scale 局部缩放
     */
    void SetLocalScale(const Vector3& scale);

    /**
     * @brief 获取世界位置
     * @return 世界位置
     */
    Vector3 GetWorldPosition() const;

    /**
     * @brief 设置世界位置
     * @param position 世界位置
     */
    void SetWorldPosition(const Vector3& position);

    /**
     * @brief 获取世界旋转
     * @return 世界旋转（四元数）
     */
    Quaternion GetWorldRotation() const;

    /**
     * @brief 设置世界旋转
     * @param rotation 世界旋转（四元数）
     */
    void SetWorldRotation(const Quaternion& rotation);

    /**
     * @brief 获取世界缩放
     * @return 世界缩放
     */
    Vector3 GetWorldScale() const;

    /**
     * @brief 设置世界缩放
     * @param scale 世界缩放
     */
    void SetWorldScale(const Vector3& scale);

    /**
     * @brief 获取局部变换矩阵
     * @return 局部变换矩阵
     */
    Matrix4 GetLocalMatrix() const;

    /**
     * @brief 获取世界变换矩阵
     * @return 世界变换矩阵
     */
    Matrix4 GetWorldMatrix() const;

    /**
     * @brief 获取前方向
     * @return 前方向（单位向量）
     */
    Vector3 GetForward() const;

    /**
     * @brief 获取右方向
     * @return 右方向（单位向量）
     */
    Vector3 GetRight() const;

    /**
     * @brief 获取上方向
     * @return 上方向（单位向量）
     */
    Vector3 GetUp() const;

    /**
     * @brief 设置父变换
     * @param parent 父变换指针
     */
    void SetParent(std::shared_ptr<Transform> parent);

    /**
     * @brief 获取父变换
     * @return 父变换指针
     */
    std::shared_ptr<Transform> GetParent() const { return m_Parent.lock(); }

    /**
     * @brief 添加子变换
     * @param child 子变换指针
     */
    void AddChild(std::shared_ptr<Transform> child);

    /**
     * @brief 移除子变换
     * @param child 子变换指针
     */
    void RemoveChild(std::shared_ptr<Transform> child);

    /**
     * @brief 获取所有子变换
     * @return 子变换列表
     */
    const std::vector<std::shared_ptr<Transform>>& GetChildren() const { return m_Children; }

private:
    Vector3 m_LocalPosition;
    Quaternion m_LocalRotation;
    Vector3 m_LocalScale;
    std::weak_ptr<Transform> m_Parent;
    std::vector<std::shared_ptr<Transform>> m_Children;
    bool m_IsDirty;
    Matrix4 m_LocalMatrix;
    Matrix4 m_WorldMatrix;

    void UpdateMatrices();
};

// 模板方法实现
template<typename T, typename... Args>
std::shared_ptr<T> Entity::AddComponent(Args&&... args) {
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

    // 检查组件是否已存在
    auto it = m_ComponentMap.find(typeid(T));
    if (it != m_ComponentMap.end()) {
        return std::dynamic_pointer_cast<T>(it->second);
    }

    // 创建新组件
    std::shared_ptr<T> component = std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...);
    m_Components.push_back(component);
    m_ComponentMap[typeid(T)] = component;
    component->Initialize();
    return component;
}

template<typename T>
std::shared_ptr<T> Entity::GetComponent() const {
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

    auto it = m_ComponentMap.find(typeid(T));
    if (it != m_ComponentMap.end()) {
        return std::dynamic_pointer_cast<T>(it->second);
    }
    return nullptr;
}

template<typename T>
bool Entity::RemoveComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

    auto it = m_ComponentMap.find(typeid(T));
    if (it != m_ComponentMap.end()) {
        auto component = it->second;
        component->Destroy();

        // 从组件列表中移除
        auto vecIt = std::find(m_Components.begin(), m_Components.end(), component);
        if (vecIt != m_Components.end()) {
            m_Components.erase(vecIt);
        }

        // 从组件映射中移除
        m_ComponentMap.erase(it);
        return true;
    }
    return false;
}

} // namespace PLE