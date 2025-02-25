#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 normalVector;
layout (location = 3) in vec2 vertexTexCoord;

uniform mat4 projectionViewMatrix;
uniform mat4 matrix6;
uniform mat4 nmatrix6;
out vec3 shaderColor;
out vec2 shaderTexCoord;
out vec3 worldSpacePosition;
out vec3 worldSpaceNormal;
out float objectType;
out mat3 shaderTBN;

void main()
{
    shaderColor = vertexColor;
    shaderTexCoord = vertexTexCoord;
    worldSpacePosition = (matrix6 * vec4(vertexPosition, 1.0f)).xyz;
    worldSpaceNormal = normalize(nmatrix6 * vec4(normalVector, 1.0f)).xyz;

    mat3 shaderTBN = mat3(1.0);

    objectType = 1.0f;

    gl_Position = projectionViewMatrix * vec4(worldSpacePosition, 1.0f);
}
