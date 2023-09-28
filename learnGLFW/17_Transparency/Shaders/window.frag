#version 330

out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D diffuse0;

void main() {
    if (texture(diffuse0, texCoord).a < 0.1) {
        discard;
    }

    FragColor = texture(diffuse0, texCoord);
}