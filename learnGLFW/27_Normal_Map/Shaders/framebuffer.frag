#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform float gammaValue;

void main()
{
    vec4 fragment = texture(screenTexture, texCoords);
    FragColor.rgb = pow(fragment.rgb, vec3(1.0f / gammaValue));
}