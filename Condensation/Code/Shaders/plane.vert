#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;

out vec2 texCoord;
out vec3 Normal;
out vec3 crntPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    // gl_Position = vec4(aPos, 1.0f);
    crntPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = camMatrix * vec4(aPos, 1.0f);
    
    texCoord = aTex;
    Normal = aNorm;
}