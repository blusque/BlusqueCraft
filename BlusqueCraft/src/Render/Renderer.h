#pragma once

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace BC
{
    class Renderer
    {
    public:
        static void Init();
        static void Render(int count);
        static void PloygonMode(bool enable);

        static glm::vec3& GetCamTrans() { return CamTrans; }
        static glm::vec3& GetCamRot() { return CamRot; }

    private:
        static inline glm::vec3 CamTrans { glm::vec3(0.f, -400.f, 0.f) };
        static inline glm::vec3 CamRot { glm::vec3(0.f, 0.f, 0.f) };
    };
}
