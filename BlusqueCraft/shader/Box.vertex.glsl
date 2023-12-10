#version 330 core

layout (location = 0) in vec4 v_Position;
layout (location = 1) in vec2 v_TextureCoords;

uniform mat4 MVP;

out vec2 o_TexCoords;

void main()
{
    gl_Position = MVP * v_Position;
    o_TexCoords = v_TextureCoords;
}