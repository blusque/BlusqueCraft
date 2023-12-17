#pragma once

#include "Render/IndexBuffer.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/VertexArray.h"

#include "Window.h"
#include "Blocks/Block.h"

#define VertexShaderFile "C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.vertex.glsl"
#define FragmentShaderFile "C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.fragment.glsl"

#ifdef _DEBUG
    #define IMGUI_DEBUG_ASSET(x) std::unordered_map<std::string, void*> (x);
#else
    #define IMGUI_DEBUG_ASSET(x)
#endif

namespace BC
{
    class Application
    {
    public:
        Application();
        virtual ~Application() = default;

        virtual void Run();

        virtual void OnEvent(Event* e);
        
    protected:
        virtual void OnBegin();
        virtual void OnTick() const;
        virtual void OnUpdate();
        virtual void OnDestroy();

        bool OnMouseScrolledEvent(MouseScrolledEvent* e);
        bool OnKeyPressedEvent(KeyPressedEvent* e);
        bool OnMouseMoveEvent(MouseMoveEvent* e);
        bool OnWindowCloseEvent(WindowCloseEvent* e);
        
        IMGUI_DEBUG_ASSET(m_ImGuiDebugAsset)
        
        WindowUPtr m_Window;
        ShaderUPtr m_Shader;
        VertexArrayUPtr m_VAO;
        std::vector<VertexBufferUPtr<Vertex>> m_VBOArr;
        std::vector<IndexBufferUPtr> m_IBOArr;
        std::vector<TextureUPtr> m_TexArr;
        bool s_Running;
        bool s_Ticked;
        float m_StartX { -1.f };
        float m_StartY { -1.f };
    };
}
