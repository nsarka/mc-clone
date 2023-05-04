#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2DArray ourTextureArray;
uniform int layer;

void main()
{
    FragColor = texture(ourTextureArray, vec3(TexCoord, float(layer)));
}