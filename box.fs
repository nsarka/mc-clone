#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
flat in float layer;

uniform sampler2DArray ourTextureArray;

void main()
{
    FragColor = texture(ourTextureArray, vec3(TexCoord, float(layer)));
}