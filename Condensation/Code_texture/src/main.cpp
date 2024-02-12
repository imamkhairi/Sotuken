#include <Model.h>
#include <heightMap.h>
#include <time.h>

const unsigned int width = 1000;
const unsigned int height = 1000;
const unsigned int texWidth = 1024;
const unsigned int texHeight = 1024;

float skyboxVertices[] =
	{
		//   Coordinates
		-1.0f, -1.0f, 1.0f,	 // 0       7--------6
		1.0f, -1.0f, 1.0f,	 // 1      /|       /|
		1.0f, -1.0f, -1.0f,	 // 2     4--------5 |
		-1.0f, -1.0f, -1.0f, // 3     | |      | |
		-1.0f, 1.0f, 1.0f,	 // 4     | 3------|-2
		1.0f, 1.0f, 1.0f,	 // 5     |/       |/
		1.0f, 1.0f, -1.0f,	 // 6     0--------1
		-1.0f, 1.0f, -1.0f	 // 7
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

std::vector<Vertex> vertices =
	{
		Vertex{glm::vec3(-1.0f, -1.0f, -0.4f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, 1.0f, -0.4f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(1.0f, 1.0f, -0.4f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, -1.0f, -0.4f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}};

// Indices for plane with texture
std::vector<GLuint> indices =
	{
		0, 1, 2,
		0, 2, 3};


int main()
{
	clock_t timer = clock();

	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile (modern functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a GLFWwindow object of 800 by 800 pixels
	GLFWwindow *window = glfwCreateWindow(width, height, "Condensation", NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	int parAmount = 1000;         // starting amount, final amount
	particleSystem ParticleSystem(parAmount, parAmount + 2000, texHeight, texWidth);
	IDMap idMap(texWidth, texHeight);

	clock_t tStart = clock();
	heightMap HeightMap(&ParticleSystem, &idMap, texHeight, texWidth);
	printf("Generate High Map: %.5f ms\n", (double)(clock() - tStart) / (CLOCKS_PER_SEC / 1000));


	glfwMakeContextCurrent(window);

	gladLoadGL();

	// Specify the viewport of OpenGL in the Window. From x = 0, y = 0, to x = width, y = height
	glViewport(0, 0, width, height);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.5f);

	glm::mat4 planeModel = glm::mat4(1.0f); // scale

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	// 1 Enable vsync / 0 disable
	glfwSwapInterval(1);

	// Generates Shader
	Shader shaderProgram("../Shaders/new.vert", "../Shaders/new.frag", "../Shaders/new.geom");
	Shader skyboxShader("../Shaders/skybox.vert", "../Shaders/skybox.frag");

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "mapSize"), (float)texWidth);
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Sky box
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::string facesCubemap[6] =
	{
		"../skybox2/right1.jpg",
		"../skybox2/left.jpg",
		"../skybox2/top.jpg",
		"../skybox2/bottom.jpg",
		"../skybox2/front.jpg",
		"../skybox2/back.jpg"
	};

	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char *data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}

	//// new Plane
	Mesh plane(vertices, indices, HeightMap.getHeightMap());

	// FPS
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;

	printf("Prep Time : %.5f ms\n", (double)(clock() - timer) / (CLOCKS_PER_SEC / 1000));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 20.0) // 
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Condensation - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			prevTime = crntTime;
			counter = 0;

			ParticleSystem.updateParticleSystem();
			
			HeightMap.generateHeightMap();
			// printf("Generate High Map: %.5f ms\n", (double)(clock() - tStart) / (CLOCKS_PER_SEC / 1000));

			plane.textures[0].Unbind();
			plane.textures[0].Delete();
			// plane.textures.clear();
			plane.textures[0] = Texture(HeightMap.getHeightMap(), "height", 2);

		}

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable depth testing since it's disabled when drawing the framebuffer rectangle
		glEnable(GL_DEPTH_TEST);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);
		// setting cam

		// Draw
		shaderProgram.Activate();
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		plane.Draw(shaderProgram, camera);

		// sky box draw
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	shaderProgram.Delete();
	skyboxShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}