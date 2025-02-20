#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
in vec3 worldSpacePosition;
in vec3 worldSpaceNormal;

uniform vec3 eyePosition;
uniform vec3 lightPosition;

uniform sampler2D shaderTexture0;
uniform sampler2D shaderTexture1;

out vec4 finalColor;

void main()
{
    vec3 l = normalize(lightPosition - worldSpacePosition); // surface to light
    vec3 e = normalize(eyePosition - worldSpacePosition); // surface to eye
    vec3 n = normalize(worldSpaceNormal); // normal vector
    float s = 8.0f; // shininess value
    vec3 r = ((-l)-2*(n*(-l))*(-l));// reflection vector

    float diffuseColor = max(dot(l, n), 0.0f);
    float ambientColor = 0.05f;
    float specularLighting = pow(max(dot(r,e),0),s);

    vec4 color0 = vec4(shaderColor*(diffuseColor+ambientColor), 1.0f)  * texture(shaderTexture0, shaderTexCoord);
    finalColor = color0;
}