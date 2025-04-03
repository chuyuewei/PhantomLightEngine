/**
 * @file RenderSystem.h
 * @brief 渲染系统定义
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../PhantomLightEngine.h"
#include "../Math/Vector.h"
#include "../Math/Matrix4.h"

namespace PLE {

// 前向声明
class Window;
class Shader;
class Texture;
class Mesh;
class Material;
class RenderTarget;
class Camera;

/**
 * @brief 渲染API类型
 */
enum class RenderAPI {
    None = 0,
    OpenGL,
    DirectX11,
    DirectX12,
    Vulkan,
    Metal
};

/**
 * @brief 渲染系统配置
 */
struct RenderSystemConfig {
    RenderAPI api = RenderAPI::OpenGL;  // 默认使用OpenGL
    bool enableVSync = true;
    bool enableMSAA = true;
    int msaaSamples = 4;
    bool enableDebugMode = false;
};

/**
 * @brief 渲染系统接口
 */
class PLE_API RenderSystem {
public:
    /**
     * @brief 创建渲染系统实例
     * @param config 渲染系统配置
     * @return 渲染系统实例
     */
    static std::unique_ptr<RenderSystem> Create(const RenderSystemConfig& config = RenderSystemConfig());

    virtual ~RenderSystem() = default;

    /**
     * @brief 初始化渲染系统
     * @param window 窗口指针
     * @return 是否成功初始化
     */
    virtual bool Initialize(std::shared_ptr<Window> window) = 0;

    /**
     * @brief 关闭渲染系统
     */
    virtual void Shutdown() = 0;

    /**
     * @brief 开始一帧的渲染
     */
    virtual void BeginFrame() = 0;

    /**
     * @brief 结束一帧的渲染
     */
    virtual void EndFrame() = 0;

    /**
     * @brief 清除缓冲区
     * @param color 清除颜色
     * @param depth 是否清除深度缓冲区
     * @param stencil 是否清除模板缓冲区
     */
    virtual void Clear(const Vector4& color, bool depth = true, bool stencil = true) = 0;

    /**
     * @brief 设置视口
     * @param x 视口左上角X坐标
     * @param y 视口左上角Y坐标
     * @param width 视口宽度
     * @param height 视口高度
     */
    virtual void SetViewport(int x, int y, int width, int height) = 0;

    /**
     * @brief 创建着色器
     * @param vertexShaderSource 顶点着色器源码
     * @param fragmentShaderSource 片段着色器源码
     * @return 着色器指针
     */
    virtual std::shared_ptr<Shader> CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) = 0;

    /**
     * @brief 创建纹理
     * @param width 纹理宽度
     * @param height 纹理高度
     * @param data 纹理数据
     * @return 纹理指针
     */
    virtual std::shared_ptr<Texture> CreateTexture(int width, int height, const void* data) = 0;

    /**
     * @brief 创建网格
     * @param vertices 顶点数据
     * @param indices 索引数据
     * @return 网格指针
     */
    virtual std::shared_ptr<Mesh> CreateMesh(const void* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount) = 0;

    /**
     * @brief 创建材质
     * @param shader 着色器指针
     * @return 材质指针
     */
    virtual std::shared_ptr<Material> CreateMaterial(std::shared_ptr<Shader> shader) = 0;

    /**
     * @brief 创建渲染目标
     * @param width 宽度
     * @param height 高度
     * @return 渲染目标指针
     */
    virtual std::shared_ptr<RenderTarget> CreateRenderTarget(int width, int height) = 0;

    /**
     * @brief 设置当前渲染目标
     * @param renderTarget 渲染目标指针，nullptr表示默认帧缓冲
     */
    virtual void SetRenderTarget(std::shared_ptr<RenderTarget> renderTarget) = 0;

    /**
     * @brief 绘制网格
     * @param mesh 网格指针
     * @param material 材质指针
     * @param transform 变换矩阵
     */
    virtual void DrawMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const Matrix4& transform) = 0;

    /**
     * @brief 设置相机
     * @param camera 相机指针
     */
    virtual void SetCamera(std::shared_ptr<Camera> camera) = 0;

    /**
     * @brief 获取当前渲染API
     * @return 渲染API类型
     */
    virtual RenderAPI GetAPI() const = 0;

    /**
     * @brief 获取GPU信息
     * @return GPU信息字符串
     */
    virtual std::string GetGPUInfo() const = 0;

    /**
     * @brief 获取API版本信息
     * @return API版本信息字符串
     */
    virtual std::string GetAPIVersion() const = 0;
};

} // namespace PLE