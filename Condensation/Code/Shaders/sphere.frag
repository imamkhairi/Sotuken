#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 crntPos;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
	    // Ambient Lighting
	// if(crntPos.z == 0.0f) discard;
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

	// Outputs final color
	// FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
	vec4 waterColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
	FragColor = (waterColor* (diffuse + ambient) + waterColor.r * specular) * lightColor;
	// FragColor = waterColor;

}