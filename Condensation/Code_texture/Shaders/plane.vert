#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    // Normal = mat3(transpose(inverse(model))) * aNormal;
    Normal = aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = camMatrix * vec4(Position, 1.0);
}  