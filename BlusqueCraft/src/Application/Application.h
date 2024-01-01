#pragma once
#include <chrono>

#include "Render/IndexBuffer.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/VertexArray.h"

#include "Core.h"
#include "Window.h"
#include "Blocks/Block.h"
#include "Blocks/Chunk.h"

#define VertexShaderFile "C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.vertex.glsl"
#define FragmentShaderFile "C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.fragment.glsl"

#ifdef _DEBUG
    #define IMGUI_DEBUG_ASSET(x) std::unordered_map<std::string, void*> (x);
#else
    #define IMGUI_DEBUG_ASSET(x)
#endif

#define TIMER_START(x) auto start##x = std::chrono::steady_clock::now();
#define TIMER_END(x) auto end##x = std::chrono::steady_clock::now();
#define SHOW_TIME_COST(x) auto duration##x = std::chrono::duration_cast<std::chrono::duration<double>>(end##x - start##x);\
    std::cout << "Time Cost " << (x) << ": " << duration##x.count() << '\n';

#define VISIBLE_CHUNK_NUM 45

namespace BC
{
    class Application
    {
    public:
        Application();
        virtual ~Application() = default;

        virtual void Run();

        virtual void OnEvent(Event* e);

        static Application* Get() { return s_Application; }
        
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

        static Application* s_Application;
        
        WindowUPtr m_Window;
        ShaderUPtr m_Shader;
        std::vector<VertexArrayUPtr> m_VAOArr;
        std::vector<VertexBufferUPtr<Vertex>> m_VBOArr;
        std::vector<IndexBufferUPtr> m_IBOArr;
        std::vector<TextureUPtr> m_TexArr;
        std::vector<ChunkPtr> m_Chunks;
        iVec2 m_CurrentChunkPos;
        bool s_Running;
        bool s_Ticked;
        float m_StartX { -1.f };
        float m_StartY { -1.f };
        float t_InsertLoc[3] { 0.f, 64.f, 0.f };
    };
}
