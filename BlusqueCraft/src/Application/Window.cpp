#include "Window.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace BC
{
    Window* Window::s_Window = nullptr;
    Window::~Window()
    {
        Shutdown();
    }

    void Window::OnUpdate() const
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    int Window::GetWindowWidth() const
    {
        return m_Data.Width;
    }

    int Window::GetWidowHeight() const
    {
        return m_Data.Height;
    }

    int Window::GLFWGetMouseButton(int button) const
    {
        return glfwGetMouseButton(m_Window, button);
    }

    void Window::SetEventCallback(const EventCallbackFn& func)
    {
        m_Data.Callback = func;
    }

    void Window::SetSync(bool enable)
    {
        if (enable)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        m_Data.VSync = enable;
    }

    bool Window::IsSync() const
    {
        return m_Data.VSync;
    }

    Window* Window::Create(const WindowProps& props)
    {
        return new Window(props);
    }

    Window::Window(const WindowProps& props)
    {
        _ASSERT(!s_Window);
        s_Window = this;
        Init(props);
    }

    void Window::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        if (!glfwInit())
            __debugbreak();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        m_Window = glfwCreateWindow(props.Width, props.Height, props.Title, nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            __debugbreak();
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetSync(true); // Enable vsync

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            auto const data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data->Width = width;
            data->Height = height;
            auto e = WindowResizeEvent(width, height);
            data->Callback(&e);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            auto const data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            auto e = WindowCloseEvent();
            data->Callback(&e);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto const data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_RELEASE:
                {
                    auto e = KeyReleasedEvent(key, mods);
                    data->Callback(&e);
                }
            case GLFW_PRESS:
                {
                    auto e = KeyPressedEvent(key, mods, 0);
                    data->Callback(&e);
                }
            case GLFW_REPEAT:
                {
                    auto e = KeyPressedEvent(key, mods, 1);
                    data->Callback(&e);
                }
            default:
                break;        
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            auto const data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_RELEASE:
                {
                    auto e = MouseButtonReleasedEvent(button);
                    data->Callback(&e);
                }
            case GLFW_PRESS:
                {
                    auto e = MouseButtonPressedEvent(button);
                    data->Callback(&e);
                }
            default:
                break;        
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            auto const data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            auto e = MouseScrolledEvent(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data->Callback(&e);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            auto const data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            auto e = MouseMoveEvent(static_cast<float>(xPos), static_cast<float>(yPos));
            data->Callback(&e);
        });

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != NULL);
    }

    void Window::Shutdown() const
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}
