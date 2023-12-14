#include "Application.h"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Render/IndexBuffer.h"
#include "Render/Renderer.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/VertexArray.h"
#include "Render/VertexArrayLayout.h"
#include "Render/VertexBuffer.h"

namespace BC
{
    Application::Application()
        : s_Running(true), s_Ticked(false)
    {
        m_Window = Renderer::Init();
        
        m_Shader = std::make_unique<Shader>(VertexShaderFile, FragmentShaderFile);
        m_Shader->Bind();

        m_VAO = std::make_unique<VertexArray>();
        m_VAO->Bind();
    }

    Application::~Application()
    {
        Renderer::Shutdown(m_Window);
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

    void Application::OnBegin()
    {
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
        // view = RotateMat4(view, camRot.x, camRot.y, camRot.z);
        int display_w, display_h;
        glfwGetFramebufferSize(m_Window, &display_w, &display_h);
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

        /* Swap front and back buffers */
        glfwSwapBuffers(m_Window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    void Application::OnDestroy() const
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
