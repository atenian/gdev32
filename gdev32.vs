#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 normalVector;
layout (location = 3) in vec2 vertexTexCoord;
layout (location = 4) in vec3 vertexTangent;

uniform mat4 projectionViewMatrix;
uniform mat4 matrix;
uniform mat4 nmatrix;
uniform float setObjectType;
uniform mat4 pointLightTransform;
out vec4 pointLightSpacePosition;
out vec3 shaderColor;
out vec2 shaderTexCoord;
out vec3 worldSpacePosition;
out vec3 worldSpaceNormal;
out float objectType;
out mat3 shaderTBN;

///////////////////////////////////////////////////////////////////////////////
// added for shadow mapping
uniform mat4 lightTransform;
out vec4 shaderLightSpacePosition;
///////////////////////////////////////////////////////////////////////////////

void main()
{
    shaderColor = vertexColor;
    shaderTexCoord = vertexTexCoord;
    worldSpacePosition = (matrix * vec4(vertexPosition, 1.0f)).xyz;
    worldSpaceNormal = normalize(nmatrix * vec4(normalVector, 1.0f)).xyz;

    vec3 worldSpaceTangent = normalize(nmatrix * vec4(vertexTangent, 1.0f)).xyz;
    vec3 worldSpaceBitangent = cross(worldSpaceNormal, worldSpaceTangent);
    shaderTBN = mat3(worldSpaceTangent, worldSpaceBitangent, worldSpaceNormal);

    objectType = setObjectType;

    gl_Position = projectionViewMatrix * vec4(worldSpacePosition, 1.0f);

    ///////////////////////////////////////////////////////////////////////////
    // also compute this fragment position from the light's point of view
    shaderLightSpacePosition = lightTransform * matrix * vec4(vertexPosition, 1.0f);
    pointLightSpacePosition = pointLightTransform * matrix * vec4(vertexPosition, 1.0f);
    ///////////////////////////////////////////////////////////////////////////
}
