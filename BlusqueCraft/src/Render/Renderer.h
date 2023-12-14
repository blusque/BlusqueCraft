#pragma once

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace BC
{
    class Renderer
    {
    public:
        static GLFWwindow* Init();
        static void Render(int count);
        static void Shutdown(GLFWwindow* window);

        static glm::vec3& GetCamTrans() { return CamTrans; }
        static glm::vec3& GetCamRot() { return CamRot; }

    private:
        static inline glm::vec3 CamTrans { glm::vec3(0.f, 0.f, -2.f) };
        static inline glm::vec3 CamRot { glm::vec3(0.f, 0.f, 0.f) };
    };
}
