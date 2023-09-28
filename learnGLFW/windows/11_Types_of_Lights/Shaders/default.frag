#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;
// Imports the normal from the vertex shader
in vec3 Normal;
// Import the current position from the Vertex Shader
in vec3 crntPos;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;

uniform sampler2D tex1;

// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light form the main function
uniform vec3 lightPos;
// Gets the position of the cammera from the main function
uniform vec3 camPos;

vec4 pointLight() {
	// used in tow variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos -crntPos;

	// intensity of light with repsect to distance
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// Ambient Lighting
	float ambient = 0.20f;

	// Diffuse Lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// Specular Lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(tex0, texCoord) * (diffuse * inten + ambient) + texture(tex1, texCoord).r * specular * inten) * lightColor;
}

vec4 directLight() {
	// Ambient Lighting
	float ambient = 0.20f;

	// Diffuse Lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f)); // this direction is the opposite of wher you wnat it to affect, i.e. down -> write up direction
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// Specular Lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(tex0, texCoord) * (diffuse  + ambient) + texture(tex1, texCoord).r * specular) * lightColor;
}

vec4 spotLight() {
	// control how big the area that is lit up is
	// cosine value of two angles
	float outerCone = 0.90f;
		// inner cone with the direction of the light
	float innerCone = 0.95f;
		// outer cone with the direction of the light

	// Ambient Lighting
	float ambient = 0.20f;

	// Diffuse Lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// Specular Lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculate the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(tex0, texCoord) * (diffuse * inten + ambient) + texture(tex1, texCoord).r * specular * inten) * lightColor;
}

void main()
{
	// Outputs final color
	FragColor = pointLight();
}