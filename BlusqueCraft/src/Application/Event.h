#pragma once

#include <functional>
#include <string>
#include <sstream>


#define BIT(x) (1 << (x))

namespace BC
{
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApp = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
    virtual EventType GetEventType() const override { return GetStaticType(); }\
    virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetEventCategory() const override { return category; }
    
    class Event
    {
        template<typename T>
        friend bool EventDispatch(Event* e, const std::function<bool(T*)>& func);
    public:
        virtual ~Event() = default;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetEventCategory() const = 0;

        virtual std::string ToString() const { return GetName(); }

        bool Handled() const { return m_Handled; }
        
    private:
        bool m_Handled { false };
    };

    template<typename T>
    using EventFn = std::function<bool(T*)>;
    
    template<typename T>
    bool EventDispatch(Event* e, const EventFn<T>& func)
    {
        if (e->GetEventType() == T::GetStaticType())
        {
            e->m_Handled = func(static_cast<T*>(e));
            return true;
        }
        return false;
    }

    // class EventDispatcher
    // {
    //     using EventFn = std::function<bool(Event*)>;
    // public:
    //     EventDispatcher(Event* e);
    //     ~EventDispatcher() = default;
    //
    //     template<typename T>
    //     bool Dispatch(const EventFn& func) const
    //     {
    //         if (m_Event->GetEventType() == T::GetStaticType())
    //         {
    //             m_Event->m_Handled = func(m_Event);
    //             return true;
    //         }
    //         return false;
    //     }
    //
    // private:
    //     Event* m_Event;
    // };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }

    class ApplicationEvent: public Event
    {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryApp)

        ApplicationEvent() = default;
    };

    class WindowCloseEvent: public ApplicationEvent
    {
    public:
        EVENT_CLASS_TYPE(WindowClose)

        WindowCloseEvent() = default;
    };
    
    class WindowResizeEvent: public ApplicationEvent
    {
    public:
        EVENT_CLASS_TYPE(WindowResize)

        unsigned int GetWidth() const { return m_Width; }
        unsigned int GetHeight() const { return m_Height; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Window is resized to X: " << m_Width
                << ", Y: " << m_Height;
            return ss.str();
        }

    public:
        WindowResizeEvent(unsigned int x, unsigned int y)
            : m_Width(x), m_Height(y) {}

    protected:
        unsigned int m_Width { 0 };
        unsigned int m_Height { 0 };
    };
    
    class WindowFocusEvent: public ApplicationEvent
    {
    public:
        EVENT_CLASS_TYPE(WindowFocus)

        WindowFocusEvent() = default;
    };
    
    class WindowLostFocusEvent: public ApplicationEvent
    {
    public:
        EVENT_CLASS_TYPE(WindowLostFocus)

        WindowLostFocusEvent() = default;
    };
    
    class WindowMovedEvent: public ApplicationEvent
    {
    public:
        EVENT_CLASS_TYPE(WindowMoved)

        unsigned int GetNewX() const { return m_X; }
        unsigned int GetNewY() const { return m_Y; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Window is moved to X: " << m_X << ", Y: " << m_Y;
            return ss.str();
        }

    public:
        WindowMovedEvent(unsigned int x, unsigned int y)
            : m_X(x), m_Y(y) {}

    protected:
        unsigned int m_X { 0 };
        unsigned int m_Y { 0 };
    };

    class AppTickEvent: public ApplicationEvent
    {
    public:
        EVENT_CLASS_TYPE(AppTick)

        AppTickEvent() = default;
    };

    class AppUpdateEvent: public ApplicationEvent
    {
    public:
        EVENT_CLASS_TYPE(AppUpdate)

        AppUpdateEvent() = default;
    };

    class AppRenderEvent: public ApplicationEvent
    {
    public:
        EVENT_CLASS_TYPE(AppRender)

        AppRenderEvent() = default;
    };

    class KeyboardEvent: public Event
    {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

        unsigned int GetKey() const { return m_KeyCode; }
        unsigned int GetMods() const { return m_Mods; }
    protected:
        KeyboardEvent(unsigned int key, unsigned int mods)
            : m_KeyCode(key), m_Mods(mods) {}
        unsigned int m_KeyCode;
        unsigned int m_Mods;
    };

    class KeyPressedEvent: public KeyboardEvent
    {
    public:
        EVENT_CLASS_TYPE(KeyPressed)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Key (" << m_KeyCode << ") pressed " << m_RepeatNum << " times!";
            return ss.str();
        }

    public:
        KeyPressedEvent(unsigned int key, unsigned int mods, unsigned int repeat)
            : KeyboardEvent(key, mods), m_RepeatNum(repeat) {}
        
    private:
        unsigned int m_RepeatNum { 0 };
    };

    class KeyReleasedEvent: public KeyboardEvent
    {
    public:
        EVENT_CLASS_TYPE(KeyReleased)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Key (" << m_KeyCode << ") released!";
            return ss.str();
        }

    public:
        KeyReleasedEvent(unsigned int key, unsigned int mods)
            : KeyboardEvent(key, mods) {}
    };

    class MouseMoveEvent: public Event
    {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
        EVENT_CLASS_TYPE(MouseMoved)

        void SetOldX(float x) { m_OldX = x; }
        void SetOldY(float y) { m_OldY = y; }
        
        float GetNewX() const { return m_X; }
        float GetNewY() const { return m_Y; }
        float GetOldX() const { return m_OldX; }
        float GetOldY() const { return m_OldY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Mouse moved to X: " << m_X << ", Y: " << m_Y;
            return ss.str();
        }

    public:
        MouseMoveEvent(float x, float y)
            : m_X(x), m_Y(y) {}
        
    private:
        float m_X { 0.f };
        float m_Y { 0.f };
        float m_OldX { -1.f };
        float m_OldY { -1.f };
    };

    class MouseScrolledEvent: public Event
    {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
        EVENT_CLASS_TYPE(MouseScrolled)

        float GetOffsetX() const { return m_OffsetX; }
        float GetOffsetY() const { return m_OffsetY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Scrolled Offset X: " << m_OffsetX << ", Offset Y: " << m_OffsetY;
            return ss.str();
        }
        
    public:
        MouseScrolledEvent(float x, float y)
            : m_OffsetX(x), m_OffsetY(y) {}

    protected:
        float m_OffsetX { 0.f };
        float m_OffsetY { 0.f };
    };

    class MouseButtonEvent: public Event
    {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

        unsigned int GetMouseButton() const { return m_ButtonCode; }

    protected:
        MouseButtonEvent(unsigned int button)
            : m_ButtonCode(button) {}
        unsigned int m_ButtonCode { 0 };
    };

    class MouseButtonPressedEvent: public MouseButtonEvent
    {
    public:
        EVENT_CLASS_TYPE(MouseButtonPressed)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Mouse button (" << m_ButtonCode << ") pressed!";
            return ss.str();
        }

    public:
        MouseButtonPressedEvent(unsigned int button)
            : MouseButtonEvent(button) {}
    };

    class MouseButtonReleasedEvent: public MouseButtonEvent
    {
    public:
        EVENT_CLASS_TYPE(MouseButtonPressed)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Mouse button (" << m_ButtonCode << ") released!";
            return ss.str();
        }

    public:
        MouseButtonReleasedEvent(unsigned int button)
            : MouseButtonEvent(button) {}
    };
}

