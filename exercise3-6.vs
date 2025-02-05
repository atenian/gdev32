#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 normalVector;
layout (location = 3) in vec2 vertexTexCoord;

uniform mat4 matrix6;
out vec3 shaderColor;
out vec2 shaderTexCoord;

void main()
{
    gl_Position = matrix6  * vec4(vertexPosition, 1.0f);
    shaderColor = vertexColor;
    shaderTexCoord = vertexTexCoord;
}
