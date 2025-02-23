#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
in vec3 worldSpacePosition;
in vec3 worldSpaceNormal;
in float objectType;

uniform vec3 eyePosition;
uniform vec3 lightPosition;
uniform sampler2D shaderTexture0;

out vec4 finalColor;

void main()
{
    // vec3 l = normalize(lightPosition - worldSpacePosition); // surface to light
    // vec3 e = normalize(eyePosition - worldSpacePosition); // surface to eye
    // vec3 n = normalize(worldSpaceNormal); // normal vector
    // float s = 8.0f; // shininess value
    // vec3 r = reflect(-l, n); // reflection vector

    // float ambient = 0.5f;
    // float diffuse = max(dot(n, l), 0.0f);
    // float specular = pow(max(dot(r,e),0),s);

    // if (objectType == 1.0f){ // if object is background, less affected by the light
    //     ambient = 0.95f;
    //     diffuse = diffuse / 5.0f;
    // }

    // vec4 texColor = texture(shaderTexture0, shaderTexCoord);
    // vec3 finalLighting = (ambient + diffuse) * shaderColor * texColor.rgb + 
    //                     specular * 0.0f;

    // finalColor = vec4(finalLighting, texColor.a);


    // Old stuff
    vec3 lightVector = normalize(lightPosition - worldSpacePosition);
    vec4 diffuseColor = dot(normalize(worldSpaceNormal), lightVector) * vec4(1.0f, 1.0f, 1.0f, 0.0f);
    diffuseColor = max(diffuseColor, 0.0f);

    vec4 ambientFactor = 1.0f * vec4(1.0, 1.0, 1.0, 0.0f);

    vec3 reflectionVector = reflect(-lightVector, normalize(worldSpaceNormal));
    float refDotEye = dot(reflectionVector, eyePosition - worldSpacePosition);
    refDotEye = max(refDotEye, 0.0f);
    vec4 specularLighting = pow(refDotEye, 2) * vec4(1.0f, 1.0f, 1.0f, 0.0f);

    vec4 texColor = texture(shaderTexture0, shaderTexCoord);
    finalColor = (diffuseColor + ambientFactor + specularLighting) * texColor * vec4(shaderColor, 1.0f);

}