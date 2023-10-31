#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{             
    // Reflection
    // vec3 I = normalize(Position - camPos);
    // vec3 R = reflect(I, normalize(Normal));
    // FragColor = vec4(texture(skybox, vec3(R.xy, -R.z)).rgb, 1.0);
    // FragColor = vec4(texture(skybox, R).rgb, 1.0);

    // Refraction
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - camPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, vec3(R.xy, -R.z)).rgb, 1.0);
}