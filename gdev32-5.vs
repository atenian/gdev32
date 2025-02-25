#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 normalVector;
layout (location = 3) in vec2 vertexTexCoord;
layout (location = 4) in vec3 vertexTangent;

uniform mat4 projectionViewMatrix;
uniform mat4 matrix5;
uniform mat4 nmatrix5;
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
    worldSpacePosition = (matrix5 * vec4(vertexPosition, 1.0f)).xyz;
    worldSpaceNormal = normalize(nmatrix5 * vec4(normalVector, 1.0f)).xyz;

    vec3 worldSpaceTangent = normalize(nmatrix5 * vec4(vertexTangent, 1.0f)).xyz;
    vec3 worldSpaceBitangent = cross(worldSpaceNormal, worldSpaceTangent);
    shaderTBN = mat3(worldSpaceTangent, worldSpaceBitangent, worldSpaceNormal);

    objectType = -1.0f;

    gl_Position = projectionViewMatrix * vec4(worldSpacePosition, 1.0f);
}
