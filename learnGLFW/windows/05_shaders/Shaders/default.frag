#version 330 core
out vec4 FragColor;

// input color from vertex shader 
in vec3 color;

uniform float scale;

void main()
{
   FragColor = vec4(color, 1.0f);
   
   // Exercise Easy (inverse the color) -> should have done here since fragment shader control the colors
   // FragColor = vec4(1.0 - color.x, 1.0 - color.y, 1.0 - color.z, 1.0f);

   // Exercise Medium (the bigger the triangle the brighter it is)
   // FragColor = vec4(color.x * scale, color.y * scale, color.z * scale, 1.0f);

}