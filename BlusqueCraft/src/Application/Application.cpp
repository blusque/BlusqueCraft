#include "Application.h"

#include <iostream>

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Render/Renderer.h"
#include "Render/VertexArrayLayout.h"
#include "Utils/Math.h"

namespace BC
{
    Application::Application()
        : s_Running(true), s_Ticked(false)
    {
        m_Window = WindowUPtr(Window::Create());
        Renderer::Init();

        m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        
        m_Shader = std::make_unique<Shader>(VertexShaderFile, FragmentShaderFile);
        m_Shader->Bind();

        m_VAO = std::make_unique<VertexArray>();
        m_VAO->Bind();
    }

    void Application::Run()
    {
        OnBegin();

        while (s_Running)
        {
            if (s_Ticked)
            {
                OnTick();
            }
            OnUpdate();
        }
    }

    void Application::OnEvent(Event* e)
    {
        EventDispatch<WindowCloseEvent>(e, std::bind(&Application::OnWindowCloseEvent, this, std::placeholders::_1));

        EventDispatch<MouseMoveEvent>(e, std::bind(&Application::OnMouseMoveEvent, this, std::placeholders::_1));

        EventDispatch<KeyPressedEvent>(e, std::bind(&Application::OnKeyPressedEvent, this, std::placeholders::_1));

        EventDispatch<MouseScrolledEvent>(e, std::bind(&Application::OnMouseScrolledEvent, this, std::placeholders::_1));
    }

    void Application::OnBegin()
    {
        float constexpr box[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        };

        unsigned int constexpr indices[] = {
            0,  1,  2,
            2,  3,  0,

            4,  5,  6,
            6,  7,  4,

            8,  9,  10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        m_VBOArr.emplace_back(std::make_unique<VertexBuffer<float>>(box, sizeof(box) / sizeof(float), VBUsage::STATIC));
        m_IBOArr.emplace_back(std::make_unique<IndexBuffer>(indices, 36));

        for (auto&& vbo : m_VBOArr)
        {
            vbo->Bind();
            auto layout = BC::VertexArrayLayout();
            layout.Add<float>(3, false);
            layout.Add<float>(2, false);
            m_VAO->SetupLayout(layout);
        }

        m_TexArr.emplace_back(std::make_unique<Texture>("C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/res/blocks/grass_side.png"));
        int i = 0;
        for (auto&& tex : m_TexArr)
        {
            tex->Bind(i);
            i++;
        }
        m_Shader->SetUniform1i("f_Texture", 0);
    }

    void Application::OnTick() const
    {
    }

    void Application::OnUpdate() const
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 1.f);

        auto constexpr model = glm::mat4(1.f);
        auto view = glm::mat4(1.f);
    
        view = rotate(view, Renderer::GetCamRot().x, glm::vec3(1.f, 0.f, 0.f));
        view = rotate(view, Renderer::GetCamRot().y, glm::vec3(0.f, 1.f, 0.f));
        view = rotate(view, Renderer::GetCamRot().z, glm::vec3(0.f, 0.f, 1.f));
        view = translate(view, Renderer::GetCamTrans());
        auto const display_w = m_Window->GetWindowWidth();
        auto const display_h = m_Window->GetWidowHeight();
        auto const width = static_cast<float>(display_w);
        auto const height = static_cast<float>(display_h);
        glViewport(0, 0, display_w, display_h);
        auto const project = glm::perspective(glm::radians(45.f), width/height, 0.1f, 100.f);

        auto mvp = project * view * model;
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("view");
        
            ImGui::Text("view =");
            ImGui::Text("%.3f\t%.3f\t%.3f\t%.3f", view[0][0], view[0][1], view[0][2], view[0][3]);
            ImGui::Text("%.3f\t%.3f\t%.3f\t%.3f", view[1][0], view[1][1], view[1][2], view[1][3]);
            ImGui::Text("%.3f\t%.3f\t%.3f\t%.3f", view[2][0], view[2][1], view[2][2], view[2][3]);
            ImGui::Text("%.3f\t%.3f\t%.3f\t%.3f", view[3][0], view[3][1], view[3][2], view[3][3]);

            ImGui::Text("Rotation: %.3f, %.3f, %.3f", Renderer::GetCamRot().x, Renderer::GetCamRot().y, Renderer::GetCamRot().z);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        m_Shader->SetUniformMatrix4fv("MVP", 1, false, &mvp[0][0]);
        for (auto&& ibo : m_IBOArr)
        {
            ibo->Bind();
            Renderer::Render(ibo->GetCount());
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_Window->OnUpdate();
    }

    void Application::OnDestroy() const
    {
    }

    bool Application::OnMouseScrolledEvent(MouseScrolledEvent* e)
    {
        auto virtualTrans = glm::vec3();
        virtualTrans.z  = 0.1f * e->GetOffsetY();
        auto const delta = RotateVec3(virtualTrans, Renderer::GetCamRot().x, Renderer::GetCamRot().y, Renderer::GetCamRot().z);
        std::cout << delta.x << ", " << delta.y << ", " << delta.z << '\n';
        Renderer::GetCamTrans() += RotateVec3(virtualTrans, -Renderer::GetCamRot().x, -Renderer::GetCamRot().y, -Renderer::GetCamRot().z);
        return true;
    }

    bool Application::OnKeyPressedEvent(KeyPressedEvent* e)
    {
        auto const key = e->GetKey();
        auto const mods = e->GetMods();
        std::cout << "key: " << key;
        if (key == 'R' || key == 'r')
        {
            std::cout << "reset!\n";
            Renderer::GetCamTrans() = glm::vec3(0.f, 0.f, -2.f);
            Renderer::GetCamRot().x = 0.f;
            Renderer::GetCamRot().y = 0.f;
            Renderer::GetCamRot().z = 0.f;
        }
        else if ((key == 'X' || key == 'x') && mods == 1)
        {
            std::cout << "-x!\n";
            Renderer::GetCamTrans() = glm::vec3(0.f, 0.f, -2.f);
            Renderer::GetCamRot().x = glm::radians(-90.f);
            Renderer::GetCamRot().y = 0.f;
            Renderer::GetCamRot().z = 0.f;
        }
        else if (key == 'X' || key == 'x')
        {
            std::cout << "x!\n";
            Renderer::GetCamTrans() = glm::vec3(0.f, 0.f, -2.f);
            Renderer::GetCamRot().x = glm::radians(90.f);
            Renderer::GetCamRot().y = 0.f;
            Renderer::GetCamRot().z = 0.f;
        }
        else if ((key == 'Y' || key == 'y') && mods == 1)
        {
            std::cout << "-y!\n";
            Renderer::GetCamTrans() = glm::vec3(0.f, 0.f, -2.f);
            Renderer::GetCamRot().x = 0.f;
            Renderer::GetCamRot().y = glm::radians(-90.f);
            Renderer::GetCamRot().z = 0.f;
        }
        else if (key == 'Y' || key == 'y')
        {
            std::cout << "y!\n";
            Renderer::GetCamTrans() = glm::vec3(0.f, 0.f, -2.f);
            Renderer::GetCamRot().x = 0.f;
            Renderer::GetCamRot().y = glm::radians(90.f);
            Renderer::GetCamRot().z = 0.f;
        }
        else if (key == 'W' || key == 'w')
        {
            auto virtualTrans = glm::vec3();
            virtualTrans.z = 0.1f;
            Renderer::GetCamTrans() += RotateVec3(virtualTrans, -Renderer::GetCamRot().x, -Renderer::GetCamRot().y, -Renderer::GetCamRot().z);
        }
        else if (key == 'S' || key == 's')
        {
            auto virtualTrans = glm::vec3();
            virtualTrans.z = -0.1f;
            Renderer::GetCamTrans() += RotateVec3(virtualTrans, -Renderer::GetCamRot().x, -Renderer::GetCamRot().y, -Renderer::GetCamRot().z);
        }
        else if (key == 'A' || key == 'a')
        {
            auto virtualTrans = glm::vec3();
            virtualTrans.x = 0.1f;
            Renderer::GetCamTrans() += RotateVec3(virtualTrans, -Renderer::GetCamRot().x, -Renderer::GetCamRot().y, -Renderer::GetCamRot().z);
        }
        else if (key == 'D' || key == 'd')
        {
            auto virtualTrans = glm::vec3();
            virtualTrans.x = -0.1f;
            Renderer::GetCamTrans() += RotateVec3(virtualTrans, -Renderer::GetCamRot().x, -Renderer::GetCamRot().y, -Renderer::GetCamRot().z);
        }
        return true;
    }

    bool Application::OnMouseMoveEvent(MouseMoveEvent* e)
    {
        auto const action0 = m_Window->GLFWGetMouseButton(GLFW_MOUSE_BUTTON_1);
        auto const action1 = m_Window->GLFWGetMouseButton(GLFW_MOUSE_BUTTON_2);
        auto const xPos = e->GetNewX();
        auto const yPos = e->GetNewY();
        if (action0)
        {
            Renderer::GetCamRot().y += static_cast<float>(glm::radians(0.5 * (xPos - m_StartX)));
            Renderer::GetCamRot().x += static_cast<float>(glm::radians(0.5 * (yPos - m_StartY)));
        }
        else if (action1)
        {
            auto virtualTrans = glm::vec3();
            virtualTrans.x = static_cast<float>(-0.01 * (xPos - m_StartX));
            virtualTrans.y = static_cast<float>(0.01 * (yPos - m_StartY));
            Renderer::GetCamTrans() += RotateVec3(virtualTrans, -Renderer::GetCamRot().x, -Renderer::GetCamRot().y, -Renderer::GetCamRot().z);
        }
        m_StartX = xPos;
        m_StartY = yPos;
        return true;
    }

    bool Application::OnWindowCloseEvent(WindowCloseEvent* e)
    {
        s_Running = false;
        return true;
    }
}
