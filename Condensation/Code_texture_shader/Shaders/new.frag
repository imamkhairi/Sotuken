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

float sobel_x[9] = float[]
(
    1, 0, -1,
    2, 0, -2,
    1, 0, -1
);

float sobel_y[9] = float[]
(
    -1, -2, -1,
     0,  0,  0,
     1,  2,  1
);

const float offset_x = 1.0f / 1024.0f; 
const float offset_y = 1.0f / 1024.0f; 

vec2 offsets[9] = vec2[](
    vec2(-offset_x, offset_y) , vec2(0.0f, offset_y) , vec2(offset_x, offset_y),
    vec2(-offset_x, 0.0f)     , vec2(0.0f, 0.0f)     , vec2(offset_x, 0.0f)    ,
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
	// vec3 heightMap = vec3(1.0f, 1.0f, 1.0f);

	// generate normal map
	// vec3 normalMap = generateNormalMap();
	
	// threshold for discarding 
	// if (texture(height0, vec2(texCoord.x, texCoord.y)).r > 0.005)
	// {
	// 	// float ratio = 1.00 / 1.52;
	// 	// vec3 I = normalize(crntPos - camPos);
	// 	// vec3 R = refract(I, normalize(normalMap), ratio);
	// 	// FragColor = vec4(texture(skybox, vec3(R.xy, -R.z)).rgb, 1.0);
	
	// }
	// else discard;

	//// Worked
	vec2 point = vec2(0.5f, 0.5f);
	float d = distance(vec2(texCoord.x, -texCoord.y), point);
	float r = 0.2;
	float value = r*r - d*d;
	float height = 0.0;
	if (value > 0.0) height = sqrt(value)/r; 

	// if (height > 0.005) {
	// } else discard;
	FragColor = vec4(vec3(height), 1.0f);


	//// Example
	// vec2 st = gl_FragCoord.xy/1024.f;
	// float pct = 0.0;
	// pct = distance(st, vec2(0.5));
	// vec3 color = vec3(pct);
	// FragColor = vec4(color, 1.0);



	///// New
	// vec2 point = vec2(0.5);
	// float height[9];
	// for (int i = 0; i < 9; i++) {
	// 	float d = distance(vec2(texCoord.x + offsets[i].x, - texCoord.y - offsets[i].y), point);
	// 	float r = 0.1;
	// 	float value = r*r - d*d;
	// 	if (value > 0.0) height[i] = sqrt(value)/r;
	// 	else height[i] = 0.0;
	// }

	// FragColor = vec4(vec3(height[4]), 1.0f);

	// vec3 normalMap = vec3(0.0f);

	// float r = 0.0f;
	// float g = 0.0f;
	// float b = 1.0f;
	// for (int i = 0; i < 9; i++) {
	// 	r += height[i] * sobel_x[i]; // salah sini
	// 	g += height[i] * sobel_y[i];
	// }
	// r = offsetAndClamp(r);
	// g = offsetAndClamp(g);

	// normalMap = vec3(r, g, b);

	// FragColor = vec4(normalMap, 1.0f);
	// if (height[4] > 0.005) {
	// 	float ratio = 1.00 / 1.52;
	// 	vec3 I = normalize(crntPos - camPos);
	// 	vec3 R = refract(I, normalize(normalMap), ratio);
	// 	FragColor = vec4(texture(skybox, vec3(R.xy, -R.z)).rgb, 1.0);
	// } else {
	// 	discard;
	// }
}