#pragma once
#include "Render/IndexBuffer.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/VertexArray.h"

#include "GLFW/glfw3.h"

#define VertexShaderFile "C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.vertex.glsl"
#define FragmentShaderFile "C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.fragment.glsl"

namespace BC
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        virtual void Run();
        
    protected:
        virtual void OnBegin();
        virtual void OnTick() const;
        virtual void OnUpdate() const;
        virtual void OnDestroy() const;

        GLFWwindow* m_Window;
        ShaderUPtr m_Shader;
        VertexArrayUPtr m_VAO;
        std::vector<VertexBufferUPtr<float>> m_VBOArr;
        std::vector<IndexBufferUPtr> m_IBOArr;
        std::vector<TextureUPtr> m_TexArr;
        bool s_Running;
        bool s_Ticked;
    };
}
