#include "Application.h"

#include <iostream>

#include "Blocks/Chunk.h"
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
    Application* Application::s_Application = nullptr;
    
    Application::Application()
        : s_Running(true), s_Ticked(false)
    {
        _ASSERT(!s_Application);
        s_Application = this;
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

        OnDestroy();
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
        m_Window = WindowUPtr(Window::Create());
        Renderer::Init();

        m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        
        m_Shader = std::make_unique<Shader>(VertexShaderFile, FragmentShaderFile);

        // auto const chunks = std::vector{
        //     Chunk(0),
        //     Chunk(1),
        //     Chunk(2),
        //     Chunk(3)
        // };
        TIMER_START(0)
        auto constexpr offset = VISIBLE_CHUNK_NUM / 2;
        for (int j = 0; j < VISIBLE_CHUNK_NUM; j++)
        {
            for (int i = 0; i < VISIBLE_CHUNK_NUM; i++)
            {
                auto const index = j * VISIBLE_CHUNK_NUM + i;
                m_Chunks.emplace_back(std::make_shared<Chunk>(i - offset, j - offset));
                if (i > 0)
                {
                    m_Chunks[index]->SetNeighbor(3, m_Chunks[index - 1]);
                    m_Chunks[index - 1]->SetNeighbor(2, m_Chunks[index]);
                }
                if (j > 0)
                {
                    m_Chunks[index]->SetNeighbor(1, m_Chunks[index - VISIBLE_CHUNK_NUM]);
                    m_Chunks[index - VISIBLE_CHUNK_NUM]->SetNeighbor(0, m_Chunks[index]);
                }
                m_Chunks[index]->GenerateBlock();
            }
        }
        TIMER_END(0)
        std::cout << "haha!\n";

        TIMER_START(1)
        auto data = std::vector<std::pair<std::vector<Vertex>, std::vector<unsigned int>>>();
        data.reserve(m_Chunks.size());
        for (auto&& chunk : m_Chunks)
        {
            auto tempData = chunk->GetVertices();
            auto tempDataIndices = chunk->GetIndices();
            data.emplace_back(tempData, tempDataIndices);
        }
        TIMER_END(1)
        std::cout << "hihi!\n";

        TIMER_START(2)
        for (auto&& datum : data)
        {
            m_VBOArr.emplace_back(std::make_unique<VertexBuffer<Vertex>>(datum.first.data(),
                datum.first.size(), VBUsage::STATIC));
            m_IBOArr.emplace_back(std::make_unique<IndexBuffer>(datum.second.data(), datum.second.size()));
        }
        TIMER_END(2)
        std::cout << "hoho!\n";
        
        SHOW_TIME_COST(0)
        SHOW_TIME_COST(1)
        SHOW_TIME_COST(2)

        TIMER_START(3)
        auto size = m_VBOArr.size();
        for (int i = 0; i < size; i++)
        {
            m_VAOArr.emplace_back(std::make_unique<VertexArray>());
            m_VBOArr[i]->Bind();
            m_IBOArr[i]->Bind();
            auto layout = VertexArrayLayout();
            layout.Add<float>(3, false);
            layout.Add<float>(2, false);
            m_VAOArr[i]->SetupLayout(layout);
        }
        TIMER_END(3)
        SHOW_TIME_COST(3)

        TIMER_START(4)
        m_TexArr.emplace_back(std::make_unique<Texture>("C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/res/block.png"));
        int i = 0;
        for (auto&& tex : m_TexArr)
        {
            tex->Bind(i);
            i++;
        }
        TIMER_END(4)
        SHOW_TIME_COST(4)
        int constexpr sampler[] = { 0, 1, 2 };
        m_Shader->SetUniform1iv("f_Texture", 3, sampler);
    }

    void Application::OnTick() const
    {
    }

    void Application::OnUpdate()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.8f, 1.f);

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
        auto io = ImGui::GetIO();
        io.DisplaySize = ImVec2(width, height);
        glViewport(0, 0, display_w, display_h);
        auto const project = glm::perspective(glm::radians(45.f), width/(height + 1e-6f), 0.1f, 100.f);

        auto mvp = project * view * model;
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("view");
            ImGui::SetWindowFontScale(2.f);
        
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
        auto const size = m_VBOArr.size();
        for (size_t i = 0; i < size; i++)
        {
            m_VAOArr[i]->Bind();
            m_VBOArr[i]->Bind();
            m_IBOArr[i]->Bind();
            Renderer::Render(m_IBOArr[i]->GetCount());
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_Window->OnUpdate();
    }

    void Application::OnDestroy()
    {
        // for (auto& v : m_ImGuiDebugAsset)
        // {
        //     std::free(v.second);
        //     v.second = nullptr;
        // }
    }

    bool Application::OnMouseScrolledEvent(MouseScrolledEvent* e)
    {
        auto virtualTrans = glm::vec3();
        virtualTrans.z  = 0.1f * e->GetOffsetY();
        auto const delta = RotateVec3(virtualTrans, Renderer::GetCamRot().x, Renderer::GetCamRot().y, Renderer::GetCamRot().z);
        Renderer::GetCamTrans() += RotateVec3(virtualTrans, -Renderer::GetCamRot().x, -Renderer::GetCamRot().y, -Renderer::GetCamRot().z);
        return true;
    }

    bool Application::OnKeyPressedEvent(KeyPressedEvent* e)
    {
        auto const key = e->GetKey();
        auto const mods = e->GetMods();
        if (key == 'R' || key == 'r')
        {
            std::cout << "reset!\n";
            Renderer::GetCamTrans() = glm::vec3(0.f, 20.f, 0.f);
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
        else if ((key == 'P' || key == 'p') && mods == 1)
        {
            Renderer::PloygonMode(false);
        }
        else if (key == 'P' || key == 'p')
        {
            Renderer::PloygonMode(true);
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
