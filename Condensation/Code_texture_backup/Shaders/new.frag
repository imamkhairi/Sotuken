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



vec4 pointLight()
{	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - crntPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 1.00f;
	float b = 0.70f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.05f;

	// diffuse lighting
	// Normals are mapped from the range [0, 1] to the range [-1, 1]
	vec3 normal = normalize(texture(normal0, texCoord).xyz * 2.0f - 1.0f);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

float sobel_x[9] = float[]
(
    1, 0, -1,
    2, 0, -2,
    1, 0, -1
);

float sobel_y[9] = float[]
(
    -1, -2, -1,
    0, 0, 0,
    1, 2, 1
);

const float offset_x = 1.0f / 1024.0f; 
const float offset_y = 1.0f / 1024.0f; 

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

	// FragColor = vec4(texture(height0, texCoord).r);
}