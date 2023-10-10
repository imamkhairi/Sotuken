#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 Normal;
out vec3 crntPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main() {
	vec3 Position = aPos;
	vec3 Norm = aNorm;

	// if(Position.z < 0.0f) {
	// 	// Position = vec3(Position.x - 0.2 * Position.x, Position.y - 0.2 * Position.y, 0.0f);
	// 	Position = vec3(Position.x, Position.y, 0.0f);
	// 	Norm = vec3(0.0f, 0.0f, -1.0f);
	// }
	// else if (Position.z < 0.02) {
	// 	Position = vec3(Position.x - 0.05 * Position.x, Position.y - 0.05 * Position.y, 0.02f);
	// }

	crntPos = vec3(model * vec4(Position, 1.0f));
	gl_Position = camMatrix * vec4(Position, 1.0f);

	Normal = Norm;
}