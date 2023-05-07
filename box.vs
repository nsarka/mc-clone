#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTexId;
layout (location = 3) in float aLightLevel;

out vec2 TexCoord;
flat out float layer;
out float lightlevel;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    layer = aTexId;
    lightlevel = aLightLevel;
}