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
uniform int pcfSize;
uniform float pcfSpread;

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

float inShadow()
{
    // perform perspective division and rescale to [0,1] range
    vec3 position = shaderLightSpacePosition.xyz / shaderLightSpacePosition.w;
    position = position * 0.5f + 0.5f;

    // if position is outside light-space frustum, not in shadow
    if (position.x < 0.0f || position.x > 1.0f ||
        position.y < 0.0f || position.y > 1.0f ||
        position.z < 0.0f || position.z > 1.0f)
    {
        return 0.0; // 0.0 means no shadow
    }

    //get current fragment depth
    float currentDepth = position.z;
    
    // add bias to prevent shadow acne
    float bias = 0.0005f;
    
    // get texture size for calculating texel size
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    // PCF implementation
    float shadow = 0.0;
    
    // use PCF kernel size based on pcfSize variable
    for(int x = -pcfSize; x <= pcfSize; ++x)
    {
        for(int y = -pcfSize; y <= pcfSize; ++y)
        {
            // Sample shadow map at offset position
            float pcfDepth = texture(shadowMap, position.xy + vec2(x, y) * texelSize * pcfSpread).r;
            
            // Compare depths
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    
    // normalize by total number of samples
    int totalSamples = (2 * pcfSize + 1) * (2 * pcfSize + 1);
    shadow /= float(totalSamples);
    
    return shadow;
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
    float shadowFactor = inShadow();

    spotDiffuseColor *= (1.0 - shadowFactor);
    spotSpecularLighting *= (1.0 - shadowFactor);
    ///////////////////////////////////////////////////////////////////////////

    // combine the lights
    vec4 finalDiffuseColor = diffuseColor + spotDiffuseColor;
    vec4 finalAmbientFactor = ambientFactor + spotAmbientFactor;
    vec4 finalSpecularLighting = specularLighting + spotSpecularLighting; 

    // refactoring conditional since it was bugged
    // For type 1 (background)
    vec4 colorType1 = ((finalDiffuseColor * 0.05f + finalAmbientFactor) * 0.36f + 0.64f + spotSpecularLighting / 48.0f) * texColor * vec4(shaderColor, 1.0f);

    // For type 2 (Kirby)
    vec4 colorType2 = (finalDiffuseColor * 1.0f + finalAmbientFactor / 2.0f + finalSpecularLighting / 8.0f) * texColor * vec4(shaderColor, 1.0f);

    // For type 3 (platforms)
    vec4 colorType3 = (finalDiffuseColor * 1.0f + finalAmbientFactor / 2.0f + finalSpecularLighting / 24.0f) * texColor * vec4(shaderColor, 1.0f);

    // Step functions to select which color to use
    float isType1 = step(0.9, objectType) * step(objectType, 1.1); // 1 if objectType is ~1.0, 0 otherwise
    float isType2 = step(1.9, objectType) * step(objectType, 2.1); // 1 if objectType is ~2.0, 0 otherwise

    // Linear interpolation - will pick one color based on the type
    vec4 finalColorTemp = colorType3 * (1.0 - isType1 - isType2) + colorType1 * isType1 + colorType2 * isType2;

    // trnasparency stuff
    float textureAlpha = texture(diffuseMap, shaderTexCoord).a;
    float warpStarAlpha = 0.48f;
    float alpha = mix(textureAlpha, warpStarAlpha, step(-5.1f, objectType) * step(objectType, -4.9f));

    // // Add alpha threshold
    // if (alpha < 0.1) {
    //     discard;
    // }

    // Apply texture alpha
    finalColor = vec4(finalColorTemp.rgb, alpha); 
}