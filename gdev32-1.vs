#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 normalVector;
layout (location = 3) in vec2 vertexTexCoord;

uniform mat4 matrix1;
uniform mat3 nmatrix1;
out vec3 shaderColor;
out vec2 shaderTexCoord;
out vec3 worldSpacePosition;
out vec3 worldSpaceNormal;

void main()
{
    gl_Position = matrix1 * vec4(vertexPosition, 1.0f);
    shaderColor = vertexColor;
    shaderTexCoord = vertexTexCoord;
    worldSpacePosition = (matrix1 * vec4(vertexPosition, 1.0f)).xyz;
    worldSpaceNormal = normalize(nmatrix1 * normalVector);
}
