/**
 * @file UISystem.h
 * @brief UI系统核心定义
 */

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "../PhantomLightEngine.h"
#include "../Math/Vector.h"
#include "../Math/Matrix4.h"
#include "../Platform/Window.h"
#include "../Renderer/RenderSystem.h"

namespace PLE {

// 前向声明
class UIElement;
class UICanvas;
class UIRenderer;

/**
 * @brief UI事件类型
 */
enum class UIEventType {
    None = 0,
    Click,
    DoubleClick,
    MouseEnter,
    MouseExit,
    MouseDown,
    MouseUp,
    MouseMove,
    MouseDrag,
    KeyDown,
    KeyUp,
    ValueChanged,
    Submit,
    Cancel,
    Focus,
    LostFocus
};

/**
 * @brief UI事件数据基类
 */
struct UIEventData {
    UIEventType type = UIEventType::None;
    UIElement* target = nullptr;
    UIElement* currentTarget = nullptr;
    bool handled = false;

    UIEventData(UIEventType type) : type(type) {}
    virtual ~UIEventData() = default;
};

/**
 * @brief 鼠标UI事件数据
 */
struct UIMouseEventData : public UIEventData {
    Vector2 position;        // 鼠标位置（相对于窗口）
    Vector2 delta;           // 鼠标移动增量
    int button = 0;          // 鼠标按钮
    int clickCount = 0;      // 点击次数

    UIMouseEventData(UIEventType type, const Vector2& position, int button = 0)
        : UIEventData(type), position(position), button(button) {}
};

/**
 * @brief 键盘UI事件数据
 */
struct UIKeyEventData : public UIEventData {
    int keyCode = 0;         // 键码
    bool alt = false;        // Alt键是否按下
    bool ctrl = false;       // Ctrl键是否按下
    bool shift = false;      // Shift键是否按下

    UIKeyEventData(UIEventType type, int keyCode)
        : UIEventData(type), keyCode(keyCode) {}
};

/**
 * @brief UI事件回调函数类型
 */
using UIEventCallback = std::function<void(UIEventData&)>;

/**
 * @brief 锚点预设类型
 */
enum class AnchorPreset {
    TopLeft,
    TopCenter,
    TopRight,
    MiddleLeft,
    MiddleCenter,
    MiddleRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
    StretchTop,
    StretchMiddle,
    StretchBottom,
    StretchLeft,
    StretchCenter,
    StretchRight,
    StretchFull
};

/**
 * @brief 矩形变换类
 * 用于控制UI元素的位置、大小、锚点和旋转
 */
class PLE_API RectTransform {
public:
    RectTransform();
    ~RectTransform() = default;

    // 位置和大小
    Vector2 GetPosition() const { return m_Position; }
    void SetPosition(const Vector2& position);
    Vector2 GetSize() const { return m_Size; }
    void SetSize(const Vector2& size);
    float GetRotation() const { return m_Rotation; }
    void SetRotation(float rotation);
    Vector2 GetScale() const { return m_Scale; }
    void SetScale(const Vector2& scale);

    // 锚点相关
    Vector2 GetAnchorMin() const { return m_AnchorMin; }
    void SetAnchorMin(const Vector2& anchorMin);
    Vector2 GetAnchorMax() const { return m_AnchorMax; }
    void SetAnchorMax(const Vector2& anchorMax);
    void SetAnchors(const Vector2& min, const Vector2& max);
    void SetAnchorPreset(AnchorPreset preset, bool preservePosition = true);

    // 边距
    Vector2 GetPivot() const { return m_Pivot; }
    void SetPivot(const Vector2& pivot);
    float GetLeft() const { return m_Offsets.x; }
    float GetRight() const { return m_Offsets.z; }
    float GetTop() const { return m_Offsets.y; }
    float GetBottom() const { return m_Offsets.w; }
    Vector4 GetOffsets() const { return m_Offsets; }
    void SetOffsets(float left, float top, float right, float bottom);
    void SetOffsets(const Vector4& offsets);

    // 变换矩阵
    Matrix4 GetLocalToWorldMatrix() const;
    Matrix4 GetWorldToLocalMatrix() const;

    // 父子关系
    void SetParent(RectTransform* parent);
    RectTransform* GetParent() const { return m_Parent; }
    const std::vector<RectTransform*>& GetChildren() const { return m_Children; }

    // 坐标转换
    Vector2 WorldToLocal(const Vector2& worldPoint) const;
    Vector2 LocalToWorld(const Vector2& localPoint) const;

    // 矩形计算
    Vector2 GetWorldPosition() const;
    Vector2 GetWorldSize() const;
    Vector4 GetWorldRect() const; // x,y为左上角，z,w为右下角

    // 更新变换
    void UpdateTransform();

private:
    Vector2 m_Position = Vector2(0.0f, 0.0f);  // 局部位置
    Vector2 m_Size = Vector2(100.0f, 100.0f);  // 大小
    float m_Rotation = 0.0f;                   // 旋转（角度）
    Vector2 m_Scale = Vector2(1.0f, 1.0f);     // 缩放

    Vector2 m_AnchorMin = Vector2(0.5f, 0.5f); // 最小锚点（0-1范围）
    Vector2 m_AnchorMax = Vector2(0.5f, 0.5f); // 最大锚点（0-1范围）
    Vector2 m_Pivot = Vector2(0.5f, 0.5f);     // 轴心点（0-1范围）
    Vector4 m_Offsets = Vector4(0.0f, 0.0f, 0.0f, 0.0f); // 左、上、右、下边距

    RectTransform* m_Parent = nullptr;
    std::vector<RectTransform*> m_Children;

    Matrix4 m_LocalToWorldMatrix = Matrix4::Identity();
    Matrix4 m_WorldToLocalMatrix = Matrix4::Identity();
    bool m_IsDirty = true;

    void AddChild(RectTransform* child);
    void RemoveChild(RectTransform* child);
};

/**
 * @brief UI元素基类
 */
class PLE_API UIElement : public std::enable_shared_from_this<UIElement> {
public:
    UIElement(const std::string& name = "UIElement");
    virtual ~UIElement();

    // 基本属性
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }
    bool IsActive() const { return m_IsActive; }
    void SetActive(bool active);
    bool IsVisible() const { return m_IsVisible; }
    void SetVisible(bool visible);
    bool IsInteractable() const { return m_IsInteractable; }
    void SetInteractable(bool interactable) { m_IsInteractable = interactable; }
    int GetSortingOrder() const { return m_SortingOrder; }
    void SetSortingOrder(int order);

    // 矩形变换
    RectTransform* GetRectTransform() { return &m_RectTransform; }
    const RectTransform* GetRectTransform() const { return &m_RectTransform; }

    // 层级关系
    UIElement* GetParent() const { return m_Parent; }
    void SetParent(UIElement* parent);
    const std::vector<std::shared_ptr<UIElement>>& GetChildren() const { return m_Children; }
    void AddChild(std::shared_ptr<UIElement> child);
    void RemoveChild(std::shared_ptr<UIElement> child);
    void RemoveAllChildren();

    // 查找
    std::shared_ptr<UIElement> FindChild(const std::string& name, bool recursive = true) const;

    // 事件处理
    void AddEventListener(UIEventType type, const UIEventCallback& callback);
    void RemoveEventListener(UIEventType type, const UIEventCallback& callback);
    bool DispatchEvent(UIEventData& eventData);

    // 生命周期
    virtual void OnEnable() {}
    virtual void OnDisable() {}
    virtual void Update(float deltaTime);
    virtual void LateUpdate(float deltaTime);
    virtual void Render(UIRenderer* renderer);

    // 碰撞检测
    bool HitTest(const Vector2& point) const;

    // 获取画布
    UICanvas* GetCanvas() const;

protected:
    std::string m_Name;
    bool m_IsActive = true;
    bool m_IsVisible = true;
    bool m_IsInteractable = true;
    int m_SortingOrder = 0;
    RectTransform m_RectTransform;
    UIElement* m_Parent = nullptr;
    std::vector<std::shared_ptr<UIElement>> m_Children;
    std::unordered_map<UIEventType, std::vector<UIEventCallback>> m_EventListeners;

    virtual void OnRender(UIRenderer* renderer) {}
    virtual bool OnHitTest(const Vector2& localPoint) const { return true; }
};

/**
 * @brief UI画布类
 * 所有UI元素的根容器，负责管理UI渲染和事件分发
 */
class PLE_API UICanvas : public UIElement {
public:
    enum class RenderMode {
        ScreenSpace,   // 屏幕空间，UI元素直接映射到屏幕
        WorldSpace,   // 世界空间，UI元素在3D世界中
        CameraSpace   // 相机空间，UI元素相对于相机
    };

    UICanvas(const std::string& name = "Canvas");
    virtual ~UICanvas();

    // 渲染模式
    RenderMode GetRenderMode() const { return m_RenderMode; }
    void SetRenderMode(RenderMode mode) { m_RenderMode = mode; }

    // 缩放模式
    float GetScaleFactor() const { return m_ScaleFactor; }
    void SetScaleFactor(float factor) { m_ScaleFactor = factor; }

    // 引用分辨率
    Vector2 GetReferenceResolution() const { return m_ReferenceResolution; }
    void SetReferenceResolution(const Vector2& resolution) { m_ReferenceResolution = resolution; }

    // 事件处理
    void ProcessWindowEvent(const WindowEventData& eventData);

    // 渲染
    virtual void Render(UIRenderer* renderer) override;

    // 更新
    virtual void Update(float deltaTime) override;
    virtual void LateUpdate(float deltaTime) override;

    // 坐标转换
    Vector2 ScreenToCanvasPoint(const Vector2& screenPoint) const;
    Vector2 CanvasToScreenPoint(const Vector2& canvasPoint) const;

private:
    RenderMode m_RenderMode = RenderMode::ScreenSpace;
    float m_ScaleFactor = 1.0f;
    Vector2 m_ReferenceResolution = Vector2(1280.0f, 720.0f);
    std::shared_ptr<UIRenderer> m_Renderer;

    // 事件处理辅助函数
    void HandleMouseEvent(const MouseEventData& eventData);
    void HandleKeyEvent(const KeyEventData& eventData);
};

/**
 * @brief UI系统类
 * 管理所有UI画布和全局UI设置
 */
class PLE_API UISystem {
public:
    static UISystem& Get();

    UISystem();
    ~UISystem();

    // 初始化和关闭
    bool Initialize(std::shared_ptr<Window> window, std::shared_ptr<RenderSystem> renderSystem);
    void Shutdown();

    // 画布管理
    std::shared_ptr<UICanvas> CreateCanvas(const std::string& name = "Canvas");
    void DestroyCanvas(std::shared_ptr<UICanvas> canvas);
    const std::vector<std::shared_ptr<UICanvas>>& GetCanvases() const { return m_Canvases; }

    // 更新和渲染
    void Update(float deltaTime);
    void Render();

    // 事件处理
    void ProcessWindowEvent(const WindowEventData& eventData);

    // 获取渲染器
    std::shared_ptr<UIRenderer> GetRenderer() const { return m_Renderer; }

private:
    std::shared_ptr<Window> m_Window;
    std::shared_ptr<RenderSystem> m_RenderSystem;
    std::shared_ptr<UIRenderer> m_Renderer;
    std::vector<std::shared_ptr<UICanvas>> m_Canvases;
    bool m_Initialized = false;
};

} // namespace PLE