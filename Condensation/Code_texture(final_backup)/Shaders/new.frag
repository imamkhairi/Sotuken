#version 330 core

out vec4 FragColor;

// Imports the current position from the Geometry Shader
in vec3 crntPos;
// Imports the normal from the Geometry Shader
in vec3 Normal;
// Imports the color from the Geometry Shader
in vec3 color;
// Imports the texture coordinates from the Geometry Shader
in vec2 texCoord;
// Gets the position of the light from the main function
in vec3 lightPos;
// Gets the position of the camera from the main function
in vec3 camPos;


// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
uniform sampler2D height0;
uniform samplerCube skybox;
// Gets the color of the light from the main function
uniform vec4 lightColor;

uniform float mapSize;

float sobel_x[9] = float[]
(
    -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1
);

float sobel_y[9] = float[]
(
    1, 2, 1,
    0, 0, 0,
    -1, -2, -1
);

float offset_x = 1.0f / mapSize; 
float offset_y = 1.0f / mapSize; 

vec2 offsets[9] = vec2[](
    vec2(-offset_x, offset_y), vec2(0.0f, offset_y), vec2(offset_x, offset_y),
    vec2(-offset_x, 0.0f), vec2(0.0f, 0.0f), vec2(offset_x, 0.0f),
    vec2(-offset_x, -offset_y), vec2(0.0f, -offset_y), vec2(offset_x, -offset_y)
);

float offsetAndClamp(float value) {
    value += 0.5f;
    if (value < 0.0f) return 0.0f;
    else if(value > 1.0f) return 1.0f;
	else return value;
}

vec3 generateNormalMap() {
	vec3 normalMap = vec3(0.0f);
	float r = 0.0f;
	float g = 0.0f;
	float b = 1.0f;
    for (int i = 0; i < 9; i++) {
        r += texture(height0, texCoord.st + offsets[i]).r * sobel_x[i];
        g += texture(height0, texCoord.st + offsets[i]).r * sobel_y[i];
    }
	r = offsetAndClamp(r);
	g = offsetAndClamp(g);
	normalMap = vec3(r, g, b);
	return normalMap;
}


void main()
{
	// outputs final color (old ver)
	// FragColor = pointLight();


	// generate normal map
	vec3 normalMap = generateNormalMap();
	
	// threshold to discard 
	if (texture(height0, vec2(texCoord.x - 1, texCoord.y)).r > 0.005)
	{
		float ratio = 1.00 / 1.52;
		vec3 I = normalize(crntPos - camPos);
		vec3 R = refract(I, normalize(normalMap), ratio);
		FragColor = vec4(texture(skybox, vec3(R.xy, -R.z)).rgb, 1.0);
		
		// FragColor = vec4(normalMap, 1.0f);
	}
	else discard;

}