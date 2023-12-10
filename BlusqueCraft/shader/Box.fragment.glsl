#version 330 core

uniform sampler2D f_Texture;

layout (location = 0) out vec4 f_Color;

in vec2 o_TexCoords;

void main()
{
    f_Color = texture(f_Texture, o_TexCoords);
    // f_Color = vec4(0.2, 0.3, 0.8, 1.0);
}