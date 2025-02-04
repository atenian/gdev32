#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;

uniform sampler2D shaderTexture0;
uniform sampler2D shaderTexture1;

uniform float mixFactor;

out vec4 finalColor;

void main()
{
    vec4 color0 = vec4(shaderColor, 1.0f) * texture(shaderTexture0, shaderTexCoord);
    finalColor = color0;
}