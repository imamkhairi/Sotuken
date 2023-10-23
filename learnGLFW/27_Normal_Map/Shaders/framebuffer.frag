#version 330

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform float gammaValue;

void main() {
    // FragColor = vec4(1.0f) - texture(screenTexture, texCoords); // inverse color
    // vec3 color = vec3(0.0f);
    // for (int i = 0; i < 9; i++) {
    //     color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];
    // }
    // FragColor = vec4(color, 1.0f);
    vec4 fragment = texture(screenTexture, texCoords); 
    FragColor.rgb = pow(fragment.rgb, vec3(1/gammaValue));
}