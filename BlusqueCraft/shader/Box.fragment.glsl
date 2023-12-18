#version 330

uniform sampler2D f_Texture[3];

layout (location = 0) out vec4 f_Color;

in vec2 o_TexCoords1;
in float o_TexId;

const float sizeX = 1.0f / 64.0f;
const float sizeY = 1.0f / 32.0f;

void main()
{
    // int index = o_TexId;
    // f_Color =
    // 猜测是OpenGL的数值不稳定的问题，表现为当传入float时，无法正确的转换为期望的int，如2.0f有时会转换为1，1.0f有时会转换成0
    f_Color = texture(f_Texture[0], vec2(o_TexCoords1.x * sizeX, o_TexCoords1.y * sizeY));
    // f_Color = mix(texture(f_Texture[0], o_TexCoords1), texture(f_Texture[1], o_TexCoords1), 0.7);
    // f_Color = vec4(1.0, 1.0, 1.0, 1.0);
}