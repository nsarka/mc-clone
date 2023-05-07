#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
flat in float layer;
in float lightlevel;

uniform sampler2DArray ourTextureArray;

void main()
{
    FragColor = ((lightlevel+1.0) / 4.0) * texture(ourTextureArray, vec3(TexCoord, float(layer)));
}