#include "Renderer.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"


namespace BC
{
    void Renderer::Init()
    {
        auto const version = gladLoadGL(glfwGetProcAddress);
        printf("GL: %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }

    void Renderer::Render(int count)
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
}
