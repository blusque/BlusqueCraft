#pragma once

#include "glm/matrix.hpp"

static glm::vec3 RotateVec3(const glm::vec3& vec, float rRoll, float rPitch, float rYaw)
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

static glm::mat4 RotateMat4(const glm::mat4& vec, float rRoll, float rPitch, float rYaw)
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