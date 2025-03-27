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
uniform sampler2D specularMap;

// Spotlight parameters
uniform vec3 lightDirection;  // Direction of the spotlight
uniform float cutOff;         // Cutoff angle 
uniform float outerCutOff;    // Outer cutoff angle 
uniform float spotIntensity;  // Intensity of the spotlight

out vec4 finalColor;

///////////////////////////////////////////////////////////////////////////////
// added for shadow mapping
in vec4 shaderLightSpacePosition;
uniform sampler2D shadowMap;

bool inShadow()
{
    // perform perspective division and rescale to the [0, 1] range to get the coordinates into the depth texture
    vec3 position = shaderLightSpacePosition.xyz / shaderLightSpacePosition.w;
    position = position * 0.5f + 0.5f;

    // if the position is outside the light-space frustum, do NOT put the
    // fragment in shadow, to prevent the scene from becoming dark "by default"
    // (note that if you have a spot light, you might want to do the opposite --
    // that is, everything outside the spot light's cone SHOULD be dark by default)
    if (position.x < 0.0f || position.x > 1.0f
        || position.y < 0.0f || position.y > 1.0f
        || position.z < 0.0f || position.z > 1.0f)
    {
        return false;
    }

    // access the shadow map at this position
    float shadowMapZ = texture(shadowMap, position.xy).r;

    // add a bias to prevent shadow acne
    float bias = 0.0005f;
    shadowMapZ += bias;

    // if the depth stored in the texture is less than the current fragment's depth, we are in shadow
    return shadowMapZ < position.z;
}
///////////////////////////////////////////////////////////////////////////////


void main()
{
    // setting up textures
    vec4 texColor = texture(diffuseMap, shaderTexCoord);
    vec3 texNormal = vec3(texture(normalMap, shaderTexCoord));
    vec4 specularIntensity = texture(specularMap, shaderTexCoord);
    texNormal = normalize(texNormal * 2.0f - 1.0f);
    vec3 normalVector = normalize(shaderTBN * texNormal);
    float specularPower = 2.0f;

    // constant properties for light
    vec3 lightVector = normalize(lightPosition - worldSpacePosition);
    vec4 diffuseColor;
    if (objectType != 1.0f){
        diffuseColor = max(dot(normalVector, lightVector), 0.0f) * vec4(1.0f, 1.0f, 1.0f, 0.0f);
    }
    else {
        diffuseColor = max(dot(normalize(worldSpaceNormal), lightVector), 0.0f) * vec4(1.0f, 1.0f, 1.0f, 0.0f);
    }

    diffuseColor = max(diffuseColor, 0.0f);

    vec4 ambientFactor = 1.0f * vec4(1.0, 1.0, 1.0, 0.0f);

    vec3 reflectionVector = reflect(-lightVector, normalize(worldSpaceNormal));
    float refDotEye = dot(reflectionVector, eyePosition - worldSpacePosition);
    refDotEye = max(refDotEye, 0.0f);
    vec4 specularLighting = pow(refDotEye, specularPower) * vec4(1.0f, 1.0f, 1.0f, 0.0f) * specularIntensity;

    // Old stuff
    vec3 spotLightVector = normalize(spotLightPosition - worldSpacePosition);
    vec4 spotDiffuseColor = dot(normalize(worldSpaceNormal), spotLightVector) * vec4(1.0f, 1.0f, 1.0f, 0.0f);
    spotDiffuseColor = max(spotDiffuseColor, 0.0f);

    vec4 spotAmbientFactor = 0.48f * vec4(1.0, 1.0, 1.0, 0.0f);

    vec3 spotReflectionVector = reflect(-spotLightVector, normalize(worldSpaceNormal));
    float spotRefDotEye = dot(spotReflectionVector, eyePosition - worldSpacePosition);
    spotRefDotEye = max(spotRefDotEye, 0.0f);
    vec4 spotSpecularLighting = pow(spotRefDotEye, specularPower) * vec4(1.0f, 1.0f, 1.0f, 0.0f) * specularIntensity;

    // Spotlight calculations
    float theta = dot(spotLightVector, normalize(-lightDirection));
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0) * spotIntensity / 0.5f;

    // Apply the spotlight effect
    spotDiffuseColor *= intensity;
    spotAmbientFactor *= intensity;
    spotSpecularLighting *= intensity;

    ///////////////////////////////////////////////////////////////////////////
    // zero-out the diffuse and specular components if the fragment is in shadow
    if (inShadow())
        spotDiffuseColor = spotSpecularLighting = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    ///////////////////////////////////////////////////////////////////////////

    // combine the lights
    vec4 finalDiffuseColor = diffuseColor + spotDiffuseColor;
    vec4 finalAmbientFactor = ambientFactor + spotAmbientFactor;
    vec4 finalSpecularLighting = specularLighting + spotSpecularLighting; 

    if (objectType == 1.0f){
        finalColor = ((finalDiffuseColor + finalAmbientFactor) * 0.36f + 0.64f) * texColor * vec4(shaderColor, 1.0f);
    }
    else if (objectType == 2.0f){
        finalColor = (finalDiffuseColor * 1.0f + finalAmbientFactor / 2.0f + finalSpecularLighting / 8.0f) * texColor * vec4(shaderColor, 1.0f);
    }
    else {
        finalColor = (finalDiffuseColor * 1.0f + finalAmbientFactor / 2.0f + finalSpecularLighting / 24.0f) * texColor * vec4(shaderColor, 1.0f);
    }
}