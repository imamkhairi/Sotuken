#include <Model.h>

const unsigned int width = 800;
const unsigned int height = 800;

// Plane Vertices
GLfloat vertices[] = 
{
	// Coordinates      	/ Texture Coordinate
	-0.5f, -0.5f, 0.5f, 	 0.0f,  0.0f,
	-0.5f,  0.5f, 0.5f, 	 0.0f,  1.0f,
	 0.5f,  0.5f, 0.5f, 	 1.0f,  1.0f,
	 0.5f, -0.5f, 0.5f, 	 1.0f,  0.0f
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

	// Tell GLFW what OpenGL version  3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("../Shaders/default.vert", "../Shaders/default.frag");
	Shader planeProgram("../Shaders/plane.vert", "../Shaders/plane.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// glm::vec4 lightColor = glm::vec4(0.3f, 0.47f, 0.78f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	planeProgram.Activate();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // option ini masih banyak yg lain, refer to documentation

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Load in a model
	Model treeModel("../Models/trees/scene.gltf");

	// Buffers
	VAO VAO;
	VAO.Bind();

	VBO VBO(vertices, sizeof(vertices));
	EBO EBO(indices, sizeof(indices));

	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void *)(0 * sizeof(float)));
	VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void *)(3 * sizeof(float)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
	
	// Texture 
	Texture test("../Textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	test.texUnit(planeProgram, "tex0", 0);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
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
		camera.Matrix(planeProgram, "camMatrix"); // give camMatrix to shader
		test.Bind();
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
	test.Delete();
	
	shaderProgram.Delete();
	planeProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}