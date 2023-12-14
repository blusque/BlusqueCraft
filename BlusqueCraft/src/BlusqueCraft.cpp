#include <iostream>
#include <fstream>

#include "Application/Application.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

int main(void)
{
    auto const app = new BC::Application;
    app->Run();
    return 0;
    /* Initialize the library */

    // glfwSetScrollCallback(window, ScrollFunc);
    // glfwSetMouseButtonCallback(window, MouseButtonFunc);
    // glfwSetCursorPosCallback(window, CursorPosFunc);
    // glfwSetKeyCallback(window, KeyFunc);
}