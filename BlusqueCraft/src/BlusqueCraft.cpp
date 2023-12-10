#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "stb_image/stb_image.h"

static const char* GL_ERR(GLenum e)
{
    switch (e)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
    default:
        return "GL_NO_ERROR";        
    }
}
static void checkGLError()
{
    auto glErr = glGetError();
    while ((glErr = glGetError()) != GL_NO_ERROR)
    {
        printf("OpenGL Error: %d %s\n", glErr, GL_ERR(glErr));
    }
}

static std::string ParserShader(const char* file)
{
    auto is = std::ifstream(file);
    auto ss = std::stringstream();
    auto line = std::string();
    if (!is.is_open())
    {
        std::cerr << "File: " << file << " is not opened!\n";
        return ss.str();
    }
    while (getline(is, line))
    {
        ss << line << '\n';
    }
    return ss.str();
}

static GLuint CreateShaderProgram(const char* vertexShader, const char* fragmentShader)
{
    auto shader = glCreateProgram();
    auto const vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertexShader, nullptr);
    glCompileShader(vShader);

    GLint vertex_compiled;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertex_compiled);
    if (vertex_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vShader, 1024, &log_length, message);
        // Write the error to a log
        printf("Vertex Shader Error: %s", message);
    }
    
    auto const fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragmentShader, nullptr);
    glCompileShader(fShader);

    GLint fragment_compiled;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragment_compiled);
    if (fragment_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fShader, 1024, &log_length, message);
        // Write the error to a log
        printf("Fragment Shader Error: %s", message);
    }

    glAttachShader(shader, vShader);
    glAttachShader(shader, fShader);
    glLinkProgram(shader);
    return shader;
}

#ifdef MC_DEBUG
    #define GLCALL(x) {\
        x;\
        checkGLError();\
        }
#else
    #define GLCALL(x) x;
#endif

#define OFFSET(x) ((const void*)(x))

auto shader = static_cast<unsigned int>(0);
auto camTrans = glm::vec3(0.f, 0.f, -2.f);
auto camRot = glm::vec3(0.f, 0.f, 0.f);
auto xStart = -1.;
auto yStart = -1.;
// auto view = glm::mat4(1.f);
auto project = glm::perspective(glm::radians(45.f), 640.f/480.f, 0.1f, 100.f);



glm::vec3 RotateVec3(const glm::vec3& vec, float rRoll, float rPitch, float rYaw)
{
    using namespace glm;
    auto const mRoll = glm::mat3(1.f,          0.f,          0.f,
                           0.f,          cos(rRoll), sin(rRoll),
                           0.f,         -sin(rRoll), cos(rRoll));

    auto const mPitch = glm::mat3(cos(rPitch), 0.f, -sin(rPitch),
                           0.f,           1.f,  0.f,
                           sin(rPitch), 0.f,  cos(rPitch));

    auto const mYaw = glm::mat3(cos(rYaw), sin(rYaw), 0.f,
                          -sin(rYaw), cos(rYaw), 0.f,
                           0.f,         0.f,         1.f);

    auto const res = mYaw * mPitch * mRoll * vec;

    return res;
}

glm::mat4 RotateMat4(const glm::mat4& vec, float rRoll, float rPitch, float rYaw)
{
    using namespace glm;
    auto const mRoll = glm::mat4(1.f,          0.f,          0.f, 0.f,
                           0.f,          cos(rRoll), -sin(rRoll), 0.f,
                           0.f,         sin(rRoll), cos(rRoll),    0.f,
                           0.f,         0.f,        0.f,          1.f);

    auto const mPitch = glm::mat4(cos(rPitch), 0.f, sin(rPitch), 0.f,
                           0.f,           1.f,  0.f, 0.f,
                           -sin(rPitch), 0.f,  cos(rRoll), 0.f,
                           0.f,          0.f,  0.f,        1.f);

    auto const mYaw = glm::mat4(cos(rYaw), -sin(rYaw), 0.f, 0.f,
                          sin(rYaw), cos(rYaw), 0.f, 0.f,
                           0.f,         0.f,         1.f, 0.f,
                           0.f,         0.f,         0.f, 1.f);

    auto const res = mYaw * mPitch * mRoll * vec;

    return res;
}

void ScrollFunc(GLFWwindow* window, double xOffset, double yOffset)
{
    auto virtualTrans = glm::vec3();
    virtualTrans.z  = (0.1f * static_cast<float>(yOffset));
    auto delta = RotateVec3(virtualTrans, camRot.x, camRot.y, camRot.z);
    std::cout << delta.x << ", " << delta.y << ", " << delta.z << '\n';
    camTrans += RotateVec3(virtualTrans, -camRot.x, -camRot.y, -camRot.z);
}

void MouseButtonFunc(GLFWwindow* window, int button, int action, int mods)
{
    // std::cout << "button: " << button << ", action: " << action << ", mods: " << mods << '\n';
}

void CursorPosFunc(GLFWwindow* window, double xPos, double yPos)
{
    auto const action0 = glfwGetMouseButton(window, 0);
    auto const action1 = glfwGetMouseButton(window, 1);
    if (action0)
    {
        camRot.y += static_cast<float>(glm::radians(0.5 * (xPos - xStart)));
        camRot.x += static_cast<float>(glm::radians(0.5 * (yPos - yStart)));
    }
    else if (action1)
    {
        auto virtualTrans = glm::vec3();
        virtualTrans.x = static_cast<float>(-0.01 * (xPos - xStart));
        virtualTrans.y = static_cast<float>(0.01 * (yPos - yStart));
        camTrans += RotateVec3(virtualTrans, -camRot.x, -camRot.y, -camRot.z);
    }
    xStart = xPos;
    yStart = yPos;
}

void KeyFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto const key_c = static_cast<unsigned char>(key);
    std::cout << "key: " << key_c << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods << '\n';
    if (key_c == 'R' || key_c == 'r')
    {
        std::cout << "reset!\n";
        camTrans = glm::vec3(0.f, 0.f, -2.f);
        camRot.x = 0.f;
        camRot.y = 0.f;
        camRot.z = 0.f;
    }
    else if ((key_c == 'X' || key_c == 'x') && mods == 1)
    {
        std::cout << "-x!\n";
        camTrans = glm::vec3(0.f, 0.f, -2.f);
        camRot.x = glm::radians(-90.f);
        camRot.y = 0.f;
        camRot.z = 0.f;
    }
    else if (key_c == 'X' || key_c == 'x')
    {
        std::cout << "x!\n";
        camTrans = glm::vec3(0.f, 0.f, -2.f);
        camRot.x = glm::radians(90.f);
        camRot.y = 0.f;
        camRot.z = 0.f;
    }
    else if ((key == 'Y' || key == 'y') && mods == 1)
    {
        std::cout << "-y!\n";
        camTrans = glm::vec3(0.f, 0.f, -2.f);
        camRot.x = 0.f;
        camRot.y = glm::radians(-90.f);
        camRot.z = 0.f;
    }
    else if (key == 'Y' || key == 'y')
    {
        std::cout << "y!\n";
        camTrans = glm::vec3(0.f, 0.f, -2.f);
        camRot.x = 0.f;
        camRot.y = glm::radians(90.f);
        camRot.z = 0.f;
    }
    else if ((key == 'W' || key == 'w') && action)
    {
        auto virtualTrans = glm::vec3();
        virtualTrans.z = 0.1f;
        camTrans += RotateVec3(virtualTrans, -camRot.x, -camRot.y, -camRot.z);
    }
    else if ((key == 'S' || key == 's') && action)
    {
        auto virtualTrans = glm::vec3();
        virtualTrans.z = -0.1f;
        camTrans += RotateVec3(virtualTrans, -camRot.x, -camRot.y, -camRot.z);
    }
    else if ((key == 'A' || key == 'a') && action)
    {
        auto virtualTrans = glm::vec3();
        virtualTrans.x = 0.1f;
        camTrans += RotateVec3(virtualTrans, -camRot.x, -camRot.y, -camRot.z);
    }
    else if ((key == 'D' || key == 'd') && action)
    {
        auto virtualTrans = glm::vec3();
        virtualTrans.x = -0.1f;
        camTrans += RotateVec3(virtualTrans, -camRot.x, -camRot.y, -camRot.z);
    }
}

GLuint LoadTexture(const char* file)
{
    int width = 0, height = 0, channels = 0;
    auto tex = stbi_load(file, &width, &height, &channels, 4);

    auto texture = static_cast<GLuint>(0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

    return texture;
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    auto window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

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
    ImGui_ImplGlfw_InitForOpenGL(window, true);
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

    auto const version = gladLoadGL(glfwGetProcAddress);
    printf("GL: %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    glEnable(GL_CULL_FACE);
    // glEnable(GL_DEPTH_TEST);

    float const box[] = {
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

    unsigned int const indices[] = {
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

    auto vbo = static_cast<GLuint>(0);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

    auto ibo = static_cast<GLuint>(0);
    auto icount = 36;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    auto vao = static_cast<GLuint>(0);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), OFFSET(3 * sizeof(float)));

    auto const vs = ParserShader("C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.vertex.glsl");
    auto const fs = ParserShader("C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/shader/Box.fragment.glsl");
    shader = CreateShaderProgram(vs.c_str(), fs.c_str());
    glUseProgram(shader);

    auto const texture = LoadTexture("C:/Users/kokut/dev/BlusqueCraft/BlusqueCraft/res/blocks/grass_side.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    auto const texLoc = glGetUniformLocation(shader, "f_Texture");
    glUniform1i(texLoc, 0);

    glfwSetScrollCallback(window, ScrollFunc);
    glfwSetMouseButtonCallback(window, MouseButtonFunc);
    glfwSetCursorPosCallback(window, CursorPosFunc);
    glfwSetKeyCallback(window, KeyFunc);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
         // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 1.f);

        auto model = glm::mat4(1.f);
        auto view = glm::mat4(1.f);
        
        view = rotate(view, camRot.x, glm::vec3(1.f, 0.f, 0.f));
        view = rotate(view, camRot.y, glm::vec3(0.f, 1.f, 0.f));
        view = rotate(view, camRot.z, glm::vec3(0.f, 0.f, 1.f));
        view = translate(view, camTrans);
        // view = RotateMat4(view, camRot.x, camRot.y, camRot.z);
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        auto const width = static_cast<float>(display_w);
        auto const height = static_cast<float>(display_h);
        glViewport(0, 0, display_w, display_h);
        // std::cout << "view: \n"; 
        // for (int i = 0; i < 4; i++)
        // {
        //     for (int j = 0; j < 4; j++)
        //     {
        //         std::cout << view[j][i] << '\t';
        //     }
        //     std::cout << '\n';
        // }
        // std::cout << "end!\n";
        project = glm::perspective(glm::radians(45.f), width/height, 0.1f, 100.f);

        auto mvp = project * view * model;
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("view");
            
            ImGui::Text("view =");
            ImGui::Text("%.3f\t%.3f\t%.3f\t%.3f", view[0][0], view[0][1], view[0][2], view[0][3]);
            ImGui::Text("%.3f\t%.3f\t%.3f\t%.3f", view[1][0], view[1][1], view[1][2], view[1][3]);
            ImGui::Text("%.3f\t%.3f\t%.3f\t%.3f", view[2][0], view[2][1], view[2][2], view[2][3]);
            ImGui::Text("%.3f\t%.3f\t%.3f\t%.3f", view[3][0], view[3][1], view[3][2], view[3][3]);

            ImGui::Text("Rotation: %.3f, %.3f, %.3f", camRot.x, camRot.y, camRot.z);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
    
        auto const location = glGetUniformLocation(shader, "MVP");
        glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, nullptr);
    
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}