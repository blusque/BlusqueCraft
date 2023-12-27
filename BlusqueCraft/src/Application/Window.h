#pragma once
#include <functional>
#include "GLFW/glfw3.h"

#include "Event.h"
#include "imgui/imgui.h"

namespace BC
{
    struct WindowProps
    {
        const char* Title;
        int Width;
        int Height;

        WindowProps(const char* title = "BlusqueCraft", int width = 1980, int height = 1080)
            : Title(title), Width(width), Height(height) {}
    };
    
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event* e)>;
    public:
        ~Window();

        void OnUpdate() const;

        void SetEventCallback(const EventCallbackFn& func);

        void SetSync(bool enable);
        bool IsSync() const;

        int GetWindowWidth() const;
        int GetWidowHeight() const;

        int GLFWGetMouseButton(int button) const;

        static Window* Create(const WindowProps& props = WindowProps());
        static Window* Get() { return s_Window; }
        
    private:
        Window(const WindowProps& props);
        void Init(const WindowProps& props);
        void Shutdown() const;

    private:
        static Window* s_Window;
        
        GLFWwindow* m_Window;
        struct WindowData
        {
            const char* Title;
            int Width;
            int Height;
            bool VSync;
            EventCallbackFn Callback;
        };
        WindowData m_Data;
    };

    using WindowPtr = std::shared_ptr<Window>;
    using WindowUPtr = std::unique_ptr<Window>;
}
