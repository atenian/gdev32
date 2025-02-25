#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
in vec3 worldSpacePosition;
in vec3 worldSpaceNormal;
in float objectType;
in mat3 shaderTBN;

uniform vec3 eyePosition;
uniform vec3 lightPosition;
uniform vec3 spotLightPosition;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

// Spotlight parameters
uniform vec3 lightDirection;  // Direction of the spotlight
uniform float cutOff;         // Cutoff angle 
uniform float outerCutOff;    // Outer cutoff angle 
uniform float spotIntensity;  // Intensity of the spotlight

out vec4 finalColor;

void main()
{
    // setting up textures
    vec4 texColor = texture(diffuseMap, shaderTexCoord);
    vec3 texNormal = vec3(texture(normalMap, shaderTexCoord));
    texNormal = normalize(texNormal * 2.0f - 1.0f);
    vec3 normalVector = normalize(shaderTBN * texNormal);

    // constant properties for light
    vec3 lightVector = normalize(lightPosition - worldSpacePosition);
    vec4 diffuseColor;
    if (objectType == -1.0f){
        diffuseColor = max(dot(normalVector, lightVector), 0.0f) * vec4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    else {
        diffuseColor = max(dot(normalize(worldSpaceNormal), lightVector), 0.0f) * vec4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    //vec4 diffuseColor = max(dot(normalize(worldSpaceNormal), lightVector), 0.0f) * vec4(1.0f, 1.0f, 1.0f, 0.0f);
    diffuseColor = max(diffuseColor, 0.0f);

    vec4 ambientFactor = 0.48f * vec4(1.0, 1.0, 1.0, 0.0f);

    vec3 reflectionVector = reflect(-lightVector, normalize(worldSpaceNormal));
    float refDotEye = dot(reflectionVector, eyePosition - worldSpacePosition);
    refDotEye = max(refDotEye, 0.0f);
    vec4 specularLighting = pow(refDotEye, 2) * vec4(1.0f, 1.0f, 1.0f, 0.0f);

    // Old stuff
    vec3 spotLightVector = normalize(spotLightPosition - worldSpacePosition);
    vec4 spotDiffuseColor = dot(normalize(worldSpaceNormal), spotLightVector) * vec4(1.0f, 1.0f, 1.0f, 0.0f);
    spotDiffuseColor = max(spotDiffuseColor, 0.0f);

    vec4 spotAmbientFactor = 0.48f * vec4(1.0, 1.0, 1.0, 0.0f);

    vec3 spotReflectionVector = reflect(-spotLightVector, normalize(worldSpaceNormal));
    float spotRefDotEye = dot(spotReflectionVector, eyePosition - worldSpacePosition);
    spotRefDotEye = max(spotRefDotEye, 0.0f);
    vec4 spotSpecularLighting = pow(spotRefDotEye, 2) * vec4(1.0f, 1.0f, 1.0f, 0.0f);

    // Spotlight calculations
    float theta = dot(spotLightVector, normalize(-lightDirection));
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0) * spotIntensity;

    // Apply the spotlight effect
    spotDiffuseColor *= intensity;
    spotAmbientFactor *= intensity;
    spotSpecularLighting *= intensity;

    // combine the lights
    vec4 finalDiffuseColor = diffuseColor + spotDiffuseColor;
    vec4 finalAmbientFactor = ambientFactor + spotAmbientFactor;
    vec4 finalSpecularLighting = specularLighting + spotSpecularLighting; 

    // if (objectType == 1.0f){
    //     finalColor = (finalAmbientFactor / 4.0f + 0.75f) * texColor * vec4(shaderColor, 1.0f);
    // }
    if (objectType == 2.0f){
        finalColor = (finalDiffuseColor + finalAmbientFactor + finalSpecularLighting / 24.0f) * texColor * vec4(shaderColor, 1.0f);
    }
    else {
        finalColor = (finalDiffuseColor + finalAmbientFactor) * texColor * vec4(shaderColor, 1.0f);
    }
}