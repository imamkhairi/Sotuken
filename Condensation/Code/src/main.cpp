#include <cmath>
#include <Model.h>

const unsigned int width = 800;
const unsigned int height = 800;

// Plane Vertices
GLfloat vertices[] = 
{
	// Coordinates      	/ Texture Coordinate  / Normals 
	-1.0f, -1.0f, 0.0f, 	 0.0f,  0.0f,		    0.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 	 0.0f,  1.0f,		    0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 	 1.0f,  1.0f,		    0.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 	 1.0f,  0.0f,		    0.0f, 0.0f, 1.0f
};

// Plane Indices
GLuint indices[] = {
	0, 2, 1, // upper triangle
	0, 3, 2  // lower triangle
};

int main()
{
	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	// Tell GLFW we are using the CORE profile (modern functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels
	GLFWwindow* window = glfwCreateWindow(width, height, "Condensation", NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::mat4 planeModel = glm::mat4(1.0f);

	// Generates Shader
	Shader shaderProgram("../Shaders/default.vert", "../Shaders/default.frag");
	Shader planeProgram("../Shaders/plane.vert", "../Shaders/plane.frag");

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	planeProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(planeProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniform4f(glGetUniformLocation(planeProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(planeProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // there are several options
	
	// face culling
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);
	// glFrontFace(GL_CCW);

	// Enable vsync
	glfwSwapInterval(1);

	// FPS
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;

	unsigned int counter = 0;




	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Load in a model
	// Model treeModel("../Models/trees/scene.gltf");



	// Buffers
	VAO VAO;
	VAO.Bind();

	VBO VBO(vertices, sizeof(vertices));
	EBO EBO(indices, sizeof(indices));

	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)(0 * sizeof(float)));
	VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, 8 * sizeof(float), (void *)(5 * sizeof(float)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
	
	// Texture 
	Texture texture("../Textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.texUnit(planeProgram, "tex0", 0);
	Texture spec("../Textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	texture.texUnit(planeProgram, "tex1", 1);

	

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0) {
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Condensation - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			prevTime = crntTime;
			counter = 0;
		}

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
			// setting cam

		// Draw a model
		// shaderProgram.Activate();
		// treeModel.Draw(shaderProgram, camera);

		planeProgram.Activate();
		glUniform3f(glGetUniformLocation(planeProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(planeProgram, "camMatrix"); // give camMatrix to shader
		texture.Bind();
		spec.Bind();
		VAO.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	// Delete all the objects we've created
	VAO.Delete();
	VBO.Delete();
	EBO.Delete();
	texture.Delete();
	
	shaderProgram.Delete();
	planeProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}