#include "Renderer.h"


namespace BC
{
    GLFWwindow* Renderer::Init()
    {
        if (!glfwInit())
            __debugbreak();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        auto const window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            __debugbreak();
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        auto const version = gladLoadGL(glfwGetProcAddress);
        printf("GL: %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

        glEnable(GL_CULL_FACE);
        // glEnable(GL_DEPTH_TEST);

        return window;
    }

    void Renderer::Render(int count)
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::Shutdown(GLFWwindow* window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
