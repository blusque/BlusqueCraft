#version 330

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TextureCoords1;
layout (location = 2) in float v_TexId;

uniform mat4 MVP;

out vec2 o_TexCoords1;
out float o_TexId;

void main()
{
    vec4 position = vec4(v_Position, 1.0);
    gl_Position = MVP * position;
    o_TexCoords1 = v_TextureCoords1;
    o_TexId = v_TexId;
}