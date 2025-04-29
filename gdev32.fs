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
uniform float alphaShift;


// Spotlight parameters
uniform vec3 lightDirection;  // Direction of the spotlight
uniform float cutOff;         // Cutoff angle 
uniform float outerCutOff;    // Outer cutoff angle 
uniform float spotIntensity;  // Intensity of the spotlight

// Cel-shading parameters (with conservative defaults)
uniform float celThreshold1 = 0.3;  // First threshold
uniform float celThreshold2 = 0.6;  // Second threshold
uniform float edgeThickness = 0.3;  // Edge thickness for outlines
uniform float celSpecularCutoff = 0.5; // Cutoff for specular highlights

out vec4 finalColor;

///////////////////////////////////////////////////////////////////////////////
// added for shadow mapping
in vec4 shaderLightSpacePosition;
in vec4 pointLightSpacePosition;
uniform sampler2D shadowMap;
uniform sampler2D pointLightShadowMap;
uniform int pcfSize;
uniform float pcfSpread;

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

float inPointLightShadow()
{
    // perform perspective division and rescale to [0,1] range
    vec3 position = pointLightSpacePosition.xyz / pointLightSpacePosition.w;
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
    vec2 texelSize = 1.0 / textureSize(pointLightShadowMap, 0);
    
    // PCF implementation
    float shadow = 0.0;
    
    // use PCF kernel size based on pcfSize variable
    for(int x = -pcfSize; x <= pcfSize; ++x)
    {
        for(int y = -pcfSize; y <= pcfSize; ++y)
        {
            // Sample shadow map at offset position
            float pcfDepth = texture(pointLightShadowMap, position.xy + vec2(x, y) * texelSize * pcfSpread).r;
            
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

// Simple cel-shading function using thresholds
float celShade(float intensity) {
    if (intensity > celThreshold2) {
        return 1.0;
    } else if (intensity > celThreshold1) {
        return 0.7;
    } else if (intensity > 0.05) {
        return 0.2;
    } else {
        return 0.0;
    }
}

// Simple specular cel-shading
float celSpecular(float intensity) {
    return intensity > celSpecularCutoff ? 1.0 : 0.0;
}

void main()
{
    // step functions to determine object type
    float isType1 = step(0.9, objectType) * step(objectType, 1.1); // 1 if objectType is ~1.0, 0 otherwise
    float isType2 = step(1.9, objectType) * step(objectType, 2.1); // 1 if objectType is ~2.0, 0 otherwise

    // setting up textures
    vec4 texColor = texture(diffuseMap, shaderTexCoord);
    vec3 texNormal = vec3(texture(normalMap, shaderTexCoord));
    vec4 specularIntensity = texture(specularMap, shaderTexCoord);
    texNormal = normalize(texNormal * 2.0f - 1.0f);
    vec3 normalVector = normalize(shaderTBN * texNormal);
    float specularPower = 2.0f;

    // constant properties for light
    vec3 lightVector = normalize(lightPosition - worldSpacePosition);
    
    // Calculate raw diffuse intensity
    float rawDiffuseIntensity;
    float isNotType1 = 1.0 - step(0.9, objectType) * step(objectType, 1.1);
    float normalDiffuse = max(dot(normalVector, lightVector), 0.0f);
    float worldNormalDiffuse = max(dot(normalize(worldSpaceNormal), lightVector), 0.0f);
    rawDiffuseIntensity = mix(worldNormalDiffuse, normalDiffuse, isNotType1);
    
    // Apply cel-shading to diffuse
    float diffuseIntensity = celShade(rawDiffuseIntensity);
    vec4 diffuseColor = diffuseIntensity * vec4(1.0f, 1.0f, 1.0f, 0.0f);

    vec4 ambientFactor = 1.0f * vec4(1.0, 1.0, 1.0, 0.0f);

    // Calculate specular
    vec3 reflectionVector = reflect(-lightVector, normalize(worldSpaceNormal));
    float refDotEye = max(dot(reflectionVector, normalize(eyePosition - worldSpacePosition)), 0.0f);
    float specIntensity = celSpecular(pow(refDotEye, specularPower));
    vec4 specularLighting = specIntensity * vec4(1.0f, 1.0f, 1.0f, 0.0f) * specularIntensity;

    // Spotlight calculations
    vec3 spotLightVector = normalize(spotLightPosition - worldSpacePosition);
    float spotRawDiffuseIntensity = max(dot(normalize(worldSpaceNormal), spotLightVector), 0.0f);
    float spotDiffuseIntensity = celShade(spotRawDiffuseIntensity);
    vec4 spotDiffuseColor = spotDiffuseIntensity * vec4(1.0f, 1.0f, 1.0f, 0.0f);

    vec4 spotAmbientFactor = 0.48f * vec4(1.0, 1.0, 1.0, 0.0f);

    // Spotlight specular
    vec3 spotReflectionVector = reflect(-spotLightVector, normalize(worldSpaceNormal));
    float spotRefDotEye = max(dot(spotReflectionVector, normalize(eyePosition - worldSpacePosition)), 0.0f);
    float spotSpecIntensity = celSpecular(pow(spotRefDotEye, specularPower));
    vec4 spotSpecularLighting = spotSpecIntensity * vec4(1.0f, 1.0f, 1.0f, 0.0f) * specularIntensity;

    // Spotlight cone effect
    float theta = dot(spotLightVector, normalize(-lightDirection));
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0) * spotIntensity / 0.5f;
    intensity = mix(intensity, intensity * 2.4f, isType1); // increased intensity for background

    // Apply the spotlight effect
    spotDiffuseColor *= intensity;
    spotAmbientFactor *= intensity;
    spotSpecularLighting *= intensity;

    ///////////////////////////////////////////////////////////////////////////
    // zero-out the diffuse and specular components if the fragment is in shadow
    float shadowFactor = inShadow();
    float pointLightShadowFactor = inPointLightShadow();
    shadowFactor = mix(shadowFactor, shadowFactor * 24.0, isType1);
    pointLightShadowFactor = mix(pointLightShadowFactor, pointLightShadowFactor * 1.48, isType1);

    spotDiffuseColor *= (1.0 - shadowFactor);
    spotSpecularLighting *= (1.0 - shadowFactor);

    diffuseColor *= (1.0 - pointLightShadowFactor);
    specularLighting *= (1.0 - pointLightShadowFactor);
    spotDiffuseColor *= (1.0 - pointLightShadowFactor);
    spotSpecularLighting *= (1.0 - pointLightShadowFactor);
    ///////////////////////////////////////////////////////////////////////////

    // combine the lights
    vec4 finalDiffuseColor = diffuseColor + spotDiffuseColor;
    vec4 finalAmbientFactor = ambientFactor + spotAmbientFactor;
    vec4 finalSpecularLighting = specularLighting + spotSpecularLighting;
    
    // Calculate outline
    vec3 viewDir = normalize(eyePosition - worldSpacePosition);
    float edgeFactor = dot(normalize(worldSpaceNormal), viewDir);
    float outline = smoothstep(0.0, edgeThickness, edgeFactor);
    
    // For type 1 (background)
    vec4 colorType1 = ((finalDiffuseColor + finalAmbientFactor) * 0.64f + 0.36f + spotSpecularLighting / 48.0f) * texColor * vec4(shaderColor, 1.0f);

    // For type 2 (Kirby)
    vec4 colorType2 = (finalDiffuseColor * 1.0f + finalAmbientFactor / 2.0f + finalSpecularLighting / 8.0f) * texColor * vec4(shaderColor, 1.0f);
    
    // For type 3 (platforms)
    vec4 colorType3 = (finalDiffuseColor * 1.0f + finalAmbientFactor / 2.0f + finalSpecularLighting / 24.0f) * texColor * vec4(shaderColor, 1.0f);

    // Apply outline - only for non-background objects
    if (objectType != 1.0f) {
        // Only apply outline to non-background objects
        colorType2 *= vec4(outline, outline, outline, 1.0);
        colorType3 *= vec4(outline, outline, outline, 1.0);
    }

    // Linear interpolation - will pick one color based on the type
    vec4 finalColorTemp = colorType3 * (1.0 - isType1 - isType2) + colorType1 * isType1 + colorType2 * isType2;

    // trnasparency stuff
    float textureAlpha = texture(diffuseMap, shaderTexCoord).a;
    float warpStarAlpha = 0.69f + alphaShift * 0.16;
    float alpha = mix(textureAlpha, warpStarAlpha, step(-5.1f, objectType) * step(objectType, -4.9f));

    // // Add alpha threshold
    // if (alpha < 0.1) {
    //     discard;
    // }

    // Apply texture alpha
    finalColor = vec4(finalColorTemp.rgb, alpha); 

    // DEBUGGING
    // point light shadow map
    // finalColor = vec4(vec3(texture(pointLightShadowMap, shaderTexCoord).r), 1.0);
    // finalColor = vec4(vec3(pointLightShadowFactor), 1.0);
    // finalColor = vec4((pointLightSpacePosition.xyz / pointLightSpacePosition.w) * 0.5 + 0.5, 1.0);
    // spotlight shadow map
    // finalColor = vec4(vec3(texture(shadowMap, shaderTexCoord).r), 1.0);
    //finalColor = vec4(vec3(shadowFactor), 1.0);
}