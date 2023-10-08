#version 330

layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNorm;

out vec3 Normal;
out vec3 crntPos;

uniform mat4 camMatrix;

void main() {
    crntPos = aPos;
    gl_Position = camMatrix * vec4(aPos, 1.0f);
    Normal = aNorm;
}