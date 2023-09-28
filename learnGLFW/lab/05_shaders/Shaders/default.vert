#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Need to output the colors from Vertex Shaders to Fragment Shader
// Outputs the color for the fragment Shader
out vec3 color;

uniform float scale;

void main()
{
   gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
               // x + x * scale -> increase the size of our triangle
   // Assigns the clors from the vertexData to "color"
   
   // Exercise Easy (flip the triangle)
   // gl_Position = vec4(aPos.x + aPos.x * scale, -aPos.y - aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
   
   color = aColor;
}