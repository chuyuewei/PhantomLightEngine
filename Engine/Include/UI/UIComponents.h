/**
 * @file UIComponents.h
 * @brief UI组件定义
 */

#pragma once

#include <memory>
#include <string>
#include <functional>

#include "UISystem.h"
#include "../Math/Vector.h"
#include "../Math/Matrix4.h"
#include "../Renderer/RenderSystem.h"

namespace PLE {

// 前向声明
class Texture;
class Font;

/**
 * @brief UI颜色类
 */
struct UIColor {
    float r, g, b, a;

    UIColor() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    UIColor(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    UIColor(const Vector4& color) : r(color.x), g(color.y), b(color.z), a(color.w) {}

    Vector4 ToVector4() const { return Vector4(r, g, b, a); }

    // 预定义颜色
    static UIColor White() { return UIColor(1.0f, 1.0f, 1.0f, 1.0f); }
    static UIColor Black() { return UIColor(0.0f, 0.0f, 0.0f, 1.0f); }
    static UIColor Red() { return UIColor(1.0f, 0.0f, 0.0f, 1.0f); }
    static UIColor Green() { return UIColor(0.0f, 1.0f, 0.0f, 1.0f); }
    static UIColor Blue() { return UIColor(0.0f, 0.0f, 1.0f, 1.0f); }
    static UIColor Yellow() { return UIColor(1.0f, 1.0f, 0.0f, 1.0f); }
    static UIColor Cyan() { return UIColor(0.0f, 1.0f, 1.0f, 1.0f); }
    static UIColor Magenta() { return UIColor(1.0f, 0.0f, 1.0f, 1.0f); }
    static UIColor Gray() { return UIColor(0.5f, 0.5f, 0.5f, 1.0f); }
    static UIColor Clear() { return UIColor(0.0f, 0.0f, 0.0f, 0.0f); }
};

/**
 * @brief 图像UI元素
 */
class PLE_API UIImage : public UIElement {
public:
    UIImage(const std::string& name = "Image");
    virtual ~UIImage() = default;

    // 纹理
    std::shared_ptr<Texture> GetTexture() const { return m_Texture; }
    void SetTexture(std::shared_ptr<Texture> texture) { m_Texture = texture; }

    // 颜色
    UIColor GetColor() const { return m_Color; }
    void SetColor(const UIColor& color) { m_Color = color; }

    // 填充方式
    enum class FillMethod {
        Simple,     // 简单填充
        Sliced,     // 九宫格填充
        Tiled,      // 平铺填充
        Filled      // 部分填充
    };

    FillMethod GetFillMethod() const { return m_FillMethod; }
    void SetFillMethod(FillMethod method) { m_FillMethod = method; }

    // 部分填充属性
    enum class FillDirection {
        LeftToRight,
        RightToLeft,
        BottomToTop,
        TopToBottom
    };

    FillDirection GetFillDirection() const { return m_FillDirection; }
    void SetFillDirection(FillDirection direction) { m_FillDirection = direction; }

    float GetFillAmount() const { return m_FillAmount; }
    void SetFillAmount(float amount) { m_FillAmount = std::max(0.0f, std::min(1.0f, amount)); }

    // 九宫格边距
    Vector4 GetBorder() const { return m_Border; }
    void SetBorder(const Vector4& border) { m_Border = border; }

    // UV坐标
    Vector4 GetUVRect() const { return m_UVRect; }
    void SetUVRect(const Vector4& uvRect) { m_UVRect = uvRect; }

protected:
    virtual void OnRender(UIRenderer* renderer) override;

private:
    std::shared_ptr<Texture> m_Texture;
    UIColor m_Color = UIColor::White();
    FillMethod m_FillMethod = FillMethod::Simple;
    FillDirection m_FillDirection = FillDirection::LeftToRight;
    float m_FillAmount = 1.0f;
    Vector4 m_Border = Vector4(0.0f, 0.0f, 0.0f, 0.0f); // 左、上、右、下边距
    Vector4 m_UVRect = Vector4(0.0f, 0.0f, 1.0f, 1.0f); // x, y, width, height
};

/**
 * @brief 文本UI元素
 */
class PLE_API UIText : public UIElement {
public:
    UIText(const std::string& name = "Text");
    virtual ~UIText() = default;

    // 文本内容
    const std::string& GetText() const { return m_Text; }
    void SetText(const std::string& text) { m_Text = text; }

    // 字体
    std::shared_ptr<Font> GetFont() const { return m_Font; }
    void SetFont(std::shared_ptr<Font> font) { m_Font = font; }

    // 字体大小
    float GetFontSize() const { return m_FontSize; }
    void SetFontSize(float size) { m_FontSize = size; }

    // 颜色
    UIColor GetColor() const { return m_Color; }
    void SetColor(const UIColor& color) { m_Color = color; }

    // 对齐方式
    enum class HorizontalAlignment {
        Left,
        Center,
        Right
    };

    enum class VerticalAlignment {
        Top,
        Middle,
        Bottom
    };

    HorizontalAlignment GetHorizontalAlignment() const { return m_HorizontalAlignment; }
    void SetHorizontalAlignment(HorizontalAlignment alignment) { m_HorizontalAlignment = alignment; }

    VerticalAlignment GetVerticalAlignment() const { return m_VerticalAlignment; }
    void SetVerticalAlignment(VerticalAlignment alignment) { m_VerticalAlignment = alignment; }

    // 文本样式
    bool IsBold() const { return m_IsBold; }
    void SetBold(bool bold) { m_IsBold = bold; }

    bool IsItalic() const { return m_IsItalic; }
    void SetItalic(bool italic) { m_IsItalic = italic; }

    bool IsUnderline() const { return m_IsUnderline; }
    void SetUnderline(bool underline) { m_IsUnderline = underline; }

    // 行间距
    float GetLineSpacing() const { return m_LineSpacing; }
    void SetLineSpacing(float spacing) { m_LineSpacing = spacing; }

    // 自动换行
    bool GetWordWrap() const { return m_WordWrap; }
    void SetWordWrap(bool wrap) { m_WordWrap = wrap; }

    // 溢出处理
    enum class OverflowMode {
        Overflow,   // 允许溢出
        Truncate,   // 截断
        Ellipsis    // 省略号
    };

    OverflowMode GetOverflowMode() const { return m_OverflowMode; }
    void SetOverflowMode(OverflowMode mode) { m_OverflowMode = mode; }

protected:
    virtual void OnRender(UIRenderer* renderer) override;

private:
    std::string m_Text;
    std::shared_ptr<Font> m_Font;
    float m_FontSize = 16.0f;
    UIColor m_Color = UIColor::Black();
    HorizontalAlignment m_HorizontalAlignment = HorizontalAlignment::Left;
    VerticalAlignment m_VerticalAlignment = VerticalAlignment::Top;
    bool m_IsBold = false;
    bool m_IsItalic = false;
    bool m_IsUnderline = false;
    float m_LineSpacing = 1.0f;
    bool m_WordWrap = true;
    OverflowMode m_OverflowMode = OverflowMode::Overflow;
};

/**
 * @brief 按钮UI元素
 */
class PLE_API UIButton : public UIElement {
public:
    UIButton(const std::string& name = "Button");
    virtual ~UIButton() = default;

    // 按钮状态
    enum class ButtonState {
        Normal,
        Highlighted,
        Pressed,
        Disabled
    };

    ButtonState GetState() const { return m_State; }

    // 背景图像
    std::shared_ptr<Texture> GetBackgroundTexture(ButtonState state) const;
    void SetBackgroundTexture(ButtonState state, std::shared_ptr<Texture> texture);

    // 背景颜色
    UIColor GetBackgroundColor(ButtonState state) const;
    void SetBackgroundColor(ButtonState state, const UIColor& color);

    // 文本
    const std::string& GetText() const { return m_Text; }
    void SetText(const std::string& text) { m_Text = text; }

    // 字体
    std::shared_ptr<Font> GetFont() const { return m_Font; }
    void SetFont(std::shared_ptr<Font> font) { m_Font = font; }

    // 字体大小
    float GetFontSize() const { return m_FontSize; }
    void SetFontSize(float size) { m_FontSize = size; }

    // 文本颜色
    UIColor GetTextColor(ButtonState state) const;
    void SetTextColor(ButtonState state, const UIColor& color);

    // 点击事件
    using ClickCallback = std::function<void()>;
    void SetOnClickCallback(const ClickCallback& callback) { m_OnClickCallback = callback; }

    // 过渡时间
    float GetTransitionDuration() const { return m_TransitionDuration; }
    void SetTransitionDuration(float duration) { m_TransitionDuration = duration; }

protected:
    virtual void OnRender(UIRenderer* renderer) override;
    virtual void Update(float deltaTime) override;

    // 事件处理
    virtual bool OnHitTest(const Vector2& localPoint) const override;

private:
    ButtonState m_State = ButtonState::Normal;
    std::string m_Text;
    std::shared_ptr<Font> m_Font;
    float m_FontSize = 16.0f;
    float m_TransitionDuration = 0.1f;
    ClickCallback m_OnClickCallback;

    // 状态相关属性
    std::shared_ptr<Texture> m_BackgroundTextures[4]; // 对应四种状态
    UIColor m_BackgroundColors[4] = {
        UIColor(0.8f, 0.8f, 0.8f, 1.0f), // Normal
        UIColor(0.9f, 0.9f, 0.9f, 1.0f), // Highlighted
        UIColor(0.7f, 0.7f, 0.7f, 1.0f), // Pressed
        UIColor(0.5f, 0.5f, 0.5f, 1.0f)  // Disabled
    };
    UIColor m_TextColors[4] = {
        UIColor::Black(), // Normal
        UIColor::Black(), // Highlighted
        UIColor::Black(), // Pressed
        UIColor(0.5f, 0.5f, 0.5f, 1.0f)  // Disabled
    };

    // 内部UI元素
    std::shared_ptr<UIImage> m_BackgroundImage;
    std::shared_ptr<UIText> m_TextLabel;

    void UpdateVisualState();
    void HandleMouseEnter(UIEventData& eventData);
    void HandleMouseExit(UIEventData& eventData);
    void HandleMouseDown(UIEventData& eventData);
    void HandleMouseUp(UIEventData& eventData);
    void HandleClick(UIEventData& eventData);
};

/**
 * @brief 输入框UI元素
 */
class PLE_API UIInputField : public UIElement {
public:
    UIInputField(const std::string& name = "InputField");
    virtual ~UIInputField() = default;

    // 文本内容
    const std::string& GetText() const { return m_Text; }
    void SetText(const std::string& text);

    // 占位符文本
    const std::string& GetPlaceholderText() const { return m_PlaceholderText; }
    void SetPlaceholderText(const std::string& text) { m_PlaceholderText = text; }

    // 字体
    std::shared_ptr<Font> GetFont() const { return m_Font; }
    void SetFont(std::shared_ptr<Font> font) { m_Font = font; }

    // 字体大小
    float GetFontSize() const { return m_FontSize; }
    void SetFontSize(float size) { m_FontSize = size; }

    // 文本颜色
    UIColor GetTextColor() const { return m_TextColor; }
    void SetTextColor(const UIColor& color) { m_TextColor = color; }

    // 占位符颜色
    UIColor GetPlaceholderColor() const { return m_PlaceholderColor; }
    void SetPlaceholderColor(const UIColor& color) { m_PlaceholderColor = color; }

    // 背景颜色
    UIColor GetBackgroundColor() const { return m_BackgroundColor; }
    void SetBackgroundColor(const UIColor& color) { m_BackgroundColor = color; }

    // 选择颜色
    UIColor GetSelectionColor() const { return m_SelectionColor; }
    void SetSelectionColor(const UIColor& color) { m_SelectionColor = color; }

    // 光标颜色
    UIColor GetCaretColor() const { return m_CaretColor; }
    void SetCaretColor(const UIColor& color) { m_CaretColor = color; }

    // 内容类型
    enum class ContentType {
        Standard,       // 标准文本
        Password,       // 密码
        Integer,        // 整数
        Decimal,        // 小数
        Alphanumeric,   // 字母数字
        Name,           // 姓名
        EmailAddress,   // 电子邮件
        Pin,            // PIN码
        Custom           // 自定义
    };

    ContentType GetContentType() const { return m_ContentType; }
    void SetContentType(ContentType type) { m_ContentType = type; }

    // 字符限制
    int GetCharacterLimit() const { return m_CharacterLimit; }
    void SetCharacterLimit(int limit) { m_CharacterLimit = limit; }

    // 是否只读
    bool IsReadOnly() const { return m_IsReadOnly; }
    void SetReadOnly(bool readOnly) { m_IsReadOnly = readOnly; }

    // 是否多行
    bool IsMultiLine() const { return m_IsMultiLine; }
    void SetMultiLine(bool multiLine) { m_IsMultiLine = multiLine; }

    // 回调函数
    using ValueChangedCallback = std::function<void(const std::string&)>;
    using SubmitCallback = std::function<void(const std::string&)>;

    void SetOnValueChangedCallback(const ValueChangedCallback& callback) { m_OnValueChangedCallback = callback; }
    void SetOnSubmitCallback(const SubmitCallback& callback) { m_OnSubmitCallback = callback; }

    // 选择范围
    void SetSelection(int start, int end);
    int GetSelectionStart() const { return m_SelectionStart; }
    int GetSelectionEnd() const { return m_SelectionEnd; }

    // 光标位置
    int GetCaretPosition() const { return m_CaretPosition; }
    void SetCaretPosition(int position);

protected:
    virtual void OnRender(UIRenderer* renderer) override;
    virtual void Update(float deltaTime) override;

    // 事件处理
    virtual bool OnHitTest(const Vector2& localPoint) const override;

private:
    std::string m_Text;
    std::string m_PlaceholderText = "Enter text...";
    std::shared_ptr<Font> m_Font;
    float m_FontSize = 16.0f;
    UIColor m_TextColor = UIColor::Black();
    UIColor m_PlaceholderColor = UIColor(0.5f, 0.5f, 0.5f, 1.0f);
    UIColor m_BackgroundColor = UIColor::White();
    UIColor m_SelectionColor = UIColor(0.2f, 0.6f, 0.9f, 0.4f);
    UIColor m_CaretColor = UIColor::Black();
    ContentType m_ContentType = ContentType::Standard;
    int m_CharacterLimit = 0; // 0表示无限制
    bool m_IsReadOnly = false;
    bool m_IsMultiLine = false;
    bool m_IsFocused = false;
    int m_CaretPosition = 0;
    int m_SelectionStart = 0;
    int m_SelectionEnd = 0;
    float m_CaretBlinkTime = 0.0f;
    bool m_CaretVisible = true;
    ValueChangedCallback m_OnValueChangedCallback;
    SubmitCallback m_OnSubmitCallback;

    // 内部UI元素
    std::shared_ptr<UIImage> m_BackgroundImage;
    std::shared_ptr<UIText> m_TextLabel;
    std::shared_ptr<UIText> m_PlaceholderLabel;

    void UpdateVisualState();
    void HandleFocus(UIEventData& eventData);
    void HandleLostFocus(UIEventData& eventData);
    void HandleKeyDown(UIEventData& eventData);
    void HandleMouseDown(UIEventData& eventData);
    void HandleValueChanged();
    void HandleSubmit();

    std::string GetDisplayText() const;
    int GetCharacterIndexFromPosition(const Vector2& localPosition) const;
};

/**
 * @brief 滑动条UI元素
 */
class PLE_API UISlider : public UIElement {
public:
    UISlider(const std::string& name = "Slider");
    virtual ~UISlider() = default;

    // 值
    float GetValue() const { return m_Value; }
    void SetValue(float value);

    // 范围
    float GetMinValue() const { return m_MinValue; }
    void SetMinValue(float minValue) { m_MinValue = minValue; }

    float GetMaxValue() const { return m_MaxValue; }
    void SetMaxValue(float maxValue) { m_MaxValue = maxValue; }

    // 方向
    enum class Direction {
        LeftToRight,
        RightToLeft,
        BottomToTop,
        TopToBottom
    };

    Direction GetDirection() const { return m_Direction; }
    void SetDirection(Direction direction) { m_Direction = direction; }

    // 是否整数
    bool IsWholeNumbers() const { return m_WholeNumbers; }
    void SetWholeNumbers(bool wholeNumbers) { m_WholeNumbers = wholeNumbers; }

    // 回调函数
    using ValueChangedCallback = std::function<void(float)>;
    void SetOnValueChangedCallback(const ValueChangedCallback& callback) { m_OnValueChangedCallback = callback; }

    // 颜色
    UIColor GetBackgroundColor() const { return m_BackgroundColor; }
    void SetBackgroundColor(const UIColor& color) { m_BackgroundColor = color; }

    UIColor GetFillColor() const { return m_FillColor; }
    void SetFillColor(const UIColor& color) { m_FillColor = color; }

    UIColor GetHandleColor() const { return m_HandleColor; }
    void SetHandleColor(const UIColor& color) { m_HandleColor = color; }

protected:
    virtual void OnRender(UIRenderer* renderer) override;
    virtual void Update(float deltaTime) override;

    // 事件处理
    virtual bool OnHitTest(const Vector2& localPoint) const override;

private:
    float m_Value = 0.0f;
    float m_MinValue = 0.0f;
    float m_MaxValue = 1.0f;
    Direction m_Direction = Direction::LeftToRight;
    bool m_WholeNumbers = false;
    bool m_IsDragging = false;
    ValueChangedCallback m_OnValueChangedCallback;
    UIColor m_BackgroundColor = UIColor(0.7f, 0.7f, 0.7f, 1.0f);
    UIColor m_FillColor = UIColor(0.2f, 0.6f, 0.9f, 1.0f);
    UIColor m_HandleColor = UIColor::White();

    // 内部UI元素
    std::shared_ptr<UIImage> m_BackgroundImage;
    std::shared_ptr<UIImage> m_FillImage;
    std::shared_ptr<UIImage> m_HandleImage;

    void UpdateVisualState();
    void HandleMouseDown(UIEventData& eventData);
    void HandleMouseUp(UIEventData& eventData);
    void HandleMouseDrag(UIEventData& eventData);
    float CalculateValueFromPosition(const Vector2& localPosition);
    Vector2 CalculatePositionFromValue(float value);
};

/**
 * @brief 复选框UI元素
 */
class PLE_API UIToggle : public UIElement {
public:
    UIToggle(const std::string& name = "Toggle");
    virtual ~UIToggle() = default;

    // 是否选中
    bool IsOn() const { return m_IsOn; }
    void SetIsOn(bool isOn);

    // 文本
    const std::string& GetText() const { return m_Text; }
    void SetText(const std::string& text) { m_Text = text; }

    // 字体
    std::shared_ptr<Font> GetFont() const { return m_Font; }
    void SetFont(std::shared_ptr<Font> font) { m_Font = font; }

    // 字体大小
    float GetFontSize() const { return m_FontSize; }
    void SetFontSize(float size) { m_FontSize = size; }

    // 颜色
    UIColor GetTextColor() const { return m_TextColor; }
    void SetTextColor(const UIColor& color) { m_TextColor = color; }

    UIColor GetBackgroundColor() const { return m_BackgroundColor; }
    void SetBackgroundColor(const UIColor& color) { m_BackgroundColor = color; }

    UIColor GetCheckmarkColor() const { return m_CheckmarkColor; }
    void SetCheckmarkColor(const UIColor& color) { m_CheckmarkColor = color; }

    // 回调函数
    using ValueChangedCallback = std::function<void(bool)>;
    void SetOnValueChangedCallback(const ValueChangedCallback& callback) { m_OnValueChangedCallback = callback; }

protected:
    virtual void OnRender(UIRenderer* renderer) override;
    virtual void Update(float deltaTime) override;

    // 事件处理
    virtual bool OnHitTest(const Vector2& localPoint) const override;

private:
    bool m_IsOn = false;
    std::string m_Text = "Toggle";
    std::shared_ptr<Font> m_Font;
    float m_FontSize = 16.0f;
    UIColor m_TextColor = UIColor::Black();
    UIColor m_BackgroundColor = UIColor::White();
    UIColor m_CheckmarkColor = UIColor(0.2f, 0.6f, 0.9f, 1.0f);
    ValueChangedCallback m_OnValueChangedCallback;

    // 内部UI元素
    std::shared_ptr<UIImage> m_BackgroundImage;
    std::shared_ptr<UIImage> m_CheckmarkImage;
    std::shared_ptr<UIText> m_TextLabel;

    void UpdateVisualState();
    void HandleClick(UIEventData& eventData);
};

/**
 * @brief 滚动视图UI元素
 */
class PLE_API UIScrollView : public UIElement {
public:
    UIScrollView(const std::string& name = "ScrollView");
    virtual ~UIScrollView() = default;

    // 内容区域
    std::shared_ptr<UIElement> GetContent() const { return m_Content; }
    void SetContent(std::shared_ptr<UIElement> content);

    // 滚动位置
    Vector2 GetScrollPosition() const { return m_ScrollPosition; }
    void SetScrollPosition(const Vector2& position);

    // 滚动范围
    Vector2 GetScrollRange() const;

    // 是否可水平滚动
    bool IsHorizontalScrollEnabled() const { return m_HorizontalScrollEnabled; }
    void SetHorizontalScrollEnabled(bool enabled) { m_HorizontalScrollEnabled = enabled; }

    // 是否可垂直滚动
    bool IsVerticalScrollEnabled() const { return m_VerticalScrollEnabled; }
    void SetVerticalScrollEnabled(bool enabled) { m_VerticalScrollEnabled = enabled; }

    // 滚动条可见性
    enum class ScrollbarVisibility {
        Always,     // 始终显示
        Auto,       // 需要时显示
        Never       // 从不显示
    };

    ScrollbarVisibility GetHorizontalScrollbarVisibility() const { return m_HorizontalScrollbarVisibility; }
    void SetHorizontalScrollbarVisibility(ScrollbarVisibility visibility) { m_HorizontalScrollbarVisibility = visibility; }

    ScrollbarVisibility GetVerticalScrollbarVisibility() const { return m_VerticalScrollbarVisibility; }
    void SetVerticalScrollbarVisibility(ScrollbarVisibility visibility) { m_VerticalScrollbarVisibility = visibility; }

    // 滚动条颜色
    UIColor GetScrollbarColor() const { return m_ScrollbarColor; }
    void SetScrollbarColor(const UIColor& color) { m_ScrollbarColor = color; }

    // 滚动速度
    float GetScrollSpeed() const { return m_ScrollSpeed; }
    void SetScrollSpeed(float speed) { m_ScrollSpeed = speed; }

    // 惯性滚动
    bool IsInertiaEnabled() const { return m_InertiaEnabled; }
    void SetInertiaEnabled(bool enabled) { m_InertiaEnabled = enabled; }

    // 弹性滚动
    bool IsElasticEnabled() const { return m_ElasticEnabled; }
    void SetElasticEnabled(bool enabled) { m_ElasticEnabled = enabled; }

protected:
    virtual void OnRender(UIRenderer* renderer) override;
    virtual void Update(float deltaTime) override;

    // 事件处理
    virtual bool OnHitTest(const Vector2& localPoint) const override;

private:
    std::shared_ptr<UIElement> m_Content;
    Vector2 m_ScrollPosition = Vector2::Zero();
    Vector2 m_ScrollVelocity = Vector2::Zero();
    bool m_HorizontalScrollEnabled = true;
    bool m_VerticalScrollEnabled = true;
    ScrollbarVisibility m_HorizontalScrollbarVisibility = ScrollbarVisibility::Auto;
    ScrollbarVisibility m_VerticalScrollbarVisibility = ScrollbarVisibility::Auto;
    UIColor m_ScrollbarColor = UIColor(0.5f, 0.5f, 0.5f, 0.5f);
    float m_ScrollSpeed = 1.0f;
    bool m_InertiaEnabled = true;
    bool m_ElasticEnabled = true;
    bool m_IsDragging = false;
    Vector2 m_DragStartPosition;
    Vector2 m_ContentStartPosition;

    // 内部UI元素
    std::shared_ptr<UIImage> m_BackgroundImage;
    std::shared_ptr<UIImage> m_HorizontalScrollbarImage;
    std::shared_ptr<UIImage> m_VerticalScrollbarImage;

    void UpdateVisualState();
    void HandleMouseDown(UIEventData& eventData);
    void HandleMouseUp(UIEventData& eventData);
    void HandleMouseDrag(UIEventData& eventData);
    void HandleMouseScroll(UIEventData& eventData);
};

/**
 * @brief 面板UI元素
 * 用于组织和布局其他UI元素
 */
class PLE_API UIPanel : public UIElement {
public:
    UIPanel(const std::string& name = "Panel");
    virtual ~UIPanel() = default;

    // 背景颜色
    UIColor GetBackgroundColor() const { return m_BackgroundColor; }
    void SetBackgroundColor(const UIColor& color) { m_BackgroundColor = color; }

    // 背景图像
    std::shared_ptr<Texture> GetBackgroundTexture() const { return m_BackgroundTexture; }
    void SetBackgroundTexture(std::shared_ptr<Texture> texture) { m_BackgroundTexture = texture; }

    // 布局类型
    enum class LayoutType {
        None,       // 无布局，子元素位置由自身决定
        Horizontal, // 水平布局
        Vertical,   // 垂直布局
        Grid        // 网格布局
    };

    LayoutType GetLayoutType() const { return m_LayoutType; }
    void SetLayoutType(LayoutType type);

    // 布局间距
    float GetSpacing() const { return m_Spacing; }
    void SetSpacing(float spacing);

    // 内边距
    Vector4 GetPadding() const { return m_Padding; }
    void SetPadding(const Vector4& padding);
    void SetPadding(float left, float top, float right, float bottom);

    // 子元素对齐方式
    enum class ChildAlignment {
        UpperLeft,
        UpperCenter,
        UpperRight,
        MiddleLeft,
        MiddleCenter,
        MiddleRight,
        LowerLeft,
        LowerCenter,
        LowerRight
    };

    ChildAlignment GetChildAlignment() const { return m_ChildAlignment; }
    void SetChildAlignment(ChildAlignment alignment);

    // 网格布局属性
    int GetGridColumns() const { return m_GridColumns; }
    void SetGridColumns(int columns);

    // 布局更新
    void UpdateLayout();

protected:
    virtual void OnRender(UIRenderer* renderer) override;
    virtual void Update(float deltaTime) override;

private:
    UIColor m_BackgroundColor = UIColor(0.2f, 0.2f, 0.2f, 0.8f);
    std::shared_ptr<Texture> m_BackgroundTexture;
    LayoutType m_LayoutType = LayoutType::None;
    float m_Spacing = 5.0f;
    Vector4 m_Padding = Vector4(5.0f, 5.0f, 5.0f, 5.0f); // 左、上、右、下内边距
    ChildAlignment m_ChildAlignment = ChildAlignment::UpperLeft;
    int m_GridColumns = 2;
    bool m_LayoutDirty = true;

    // 内部UI元素
    std::shared_ptr<UIImage> m_BackgroundImage;

    void UpdateHorizontalLayout();
    void UpdateVerticalLayout();
    void UpdateGridLayout();
};

} // namespace PLE