#pragma once

#include "Render/IndexBuffer.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/VertexArray.h"

#include "Window.h"

#define VertexShaderFile "C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.vertex.glsl"
#define FragmentShaderFile "C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.fragment.glsl"

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
        virtual void OnUpdate() const;
        virtual void OnDestroy() const;

        bool OnMouseScrolledEvent(MouseScrolledEvent* e);
        bool OnKeyPressedEvent(KeyPressedEvent* e);
        bool OnMouseMoveEvent(MouseMoveEvent* e);
        bool OnWindowCloseEvent(WindowCloseEvent* e);

        WindowUPtr m_Window;
        ShaderUPtr m_Shader;
        VertexArrayUPtr m_VAO;
        std::vector<VertexBufferUPtr<float>> m_VBOArr;
        std::vector<IndexBufferUPtr> m_IBOArr;
        std::vector<TextureUPtr> m_TexArr;
        bool s_Running;
        bool s_Ticked;
        float m_StartX { -1.f };
        float m_StartY { -1.f };
    };
}
