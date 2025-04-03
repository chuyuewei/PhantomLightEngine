/**
 * @file PhysicsSystem.h
 * @brief 物理系统定义
 */

#pragma once

#include <memory>
#include <vector>

#include "../PhantomLightEngine.h"
#include "../Math/Vector.h"

namespace PLE {

// 前向声明
class RigidBody;
class Collider;
class PhysicsScene;

/**
 * @brief 物理系统配置
 */
struct PhysicsConfig {
    Vector3 gravity = Vector3(0.0f, -9.81f, 0.0f);
    float fixedTimeStep = 1.0f / 60.0f;
    int maxSubSteps = 10;
    bool enableCCD = true;
    bool enableDebugDraw = false;
};

/**
 * @brief 物理系统类
 */
class PLE_API PhysicsSystem {
public:
    /**
     * @brief 创建物理系统实例
     * @param config 物理系统配置
     * @return 物理系统实例
     */
    static std::unique_ptr<PhysicsSystem> Create(const PhysicsConfig& config = PhysicsConfig());

    virtual ~PhysicsSystem() = default;

    /**
     * @brief 初始化物理系统
     * @return 是否成功初始化
     */
    virtual bool Initialize() = 0;

    /**
     * @brief 关闭物理系统
     */
    virtual void Shutdown() = 0;

    /**
     * @brief 更新物理系统
     * @param deltaTime 帧时间间隔（秒）
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief 创建物理场景
     * @return 物理场景指针
     */
    virtual std::shared_ptr<PhysicsScene> CreateScene() = 0;

    /**
     * @brief 创建刚体
     * @param mass 质量
     * @param position 位置
     * @param rotation 旋转
     * @return 刚体指针
     */
    virtual std::shared_ptr<RigidBody> CreateRigidBody(float mass, const Vector3& position, const Vector3& rotation) = 0;

    /**
     * @brief 创建盒体碰撞器
     * @param halfExtents 半尺寸
     * @return 碰撞器指针
     */
    virtual std::shared_ptr<Collider> CreateBoxCollider(const Vector3& halfExtents) = 0;