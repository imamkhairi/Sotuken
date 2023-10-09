#include <Model.h>
#include <icosphere.h>

const unsigned int width = 1000;
const unsigned int height = 1000;

// Plane Vertices
float vertices[] = 
{
	// Coordinates      	/ Texture Coordinate  / Normals 
	-1.0f, -1.0f, 0.0f, 	 0.0f,  0.0f,		    0.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 	 0.0f,  1.0f,		    0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 	 1.0f,  1.0f,		    0.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 	 1.0f,  0.0f,		    0.0f, 0.0f, 1.0f
};

// Plane Indices
unsigned int indices[] = {
	0, 2, 1, // upper triangle
	0, 3, 2  // lower triangle
};

float spherevertices[] = {
	0, 0, 1, 3.11266e-08, -0.607062, 0.794654, 
	-0.525731, -0.723607, 0.447214, 3.11266e-08, -0.607062, 0.794654,
	0.525731, -0.723607, 0.447214, 3.11266e-08, -0.607062, 0.794654,
	-0.525731, -0.723607, 0.447214, 5.5681e-08, -0.982247, 0.187592,
	-3.90967e-08, -0.894427, -0.447214, 5.5681e-08, -0.982247, 0.187592,
	0.525731, -0.723607, 0.447214, 5.5681e-08, -0.982247, 0.187592,
	0.525731, -0.723607, 0.447214, 0.57735, -0.794654, -0.187592,
	-3.90967e-08, -0.894427, -0.447214, 0.57735, -0.794654, -0.187592,
	0.850651, -0.276393, -0.447214, 0.57735, -0.794654, -0.187592,
	-3.90967e-08, -0.894427, -0.447214, 0.356822, -0.491123, -0.794654,
	0, 0, -1, 0.356822, -0.491123, -0.794654,
	0.850651, -0.276393, -0.447214, 0.356822, -0.491123, -0.794654,
	0, 0, 1, 0.57735, -0.187592, 0.794654,
	0.525731, -0.723607, 0.447214, 0.57735, -0.187592, 0.794654,
	0.850651, 0.276393, 0.447214, 0.57735, -0.187592, 0.794654,
	0.525731, -0.723607, 0.447214, 0.934172, -0.303531, 0.187593,
	0.850651, -0.276393, -0.447214, 0.934172, -0.303531, 0.187593,
	0.850651, 0.276393, 0.447214, 0.934172, -0.303531, 0.187593,
	0.850651, 0.276393, 0.447214, 0.934172, 0.303531, -0.187592,
	0.850651, -0.276393, -0.447214, 0.934172, 0.303531, -0.187592,
	0.525731, 0.723607, -0.447214, 0.934172, 0.303531, -0.187592,
	0.850651, -0.276393, -0.447214, 0.57735, 0.187593, -0.794654,
	0, 0, -1, 0.57735, 0.187593, -0.794654, 
	0.525731, 0.723607, -0.447214, 0.57735, 0.187593, -0.794654,
	0, 0, 1, 0.356822, 0.491124, 0.794654,
	0.850651, 0.276393, 0.447214, 0.356822, 0.491124, 0.794654,
	-1.45721e-07, 0.894427, 0.447214, 0.356822, 0.491124, 0.794654,
	0.850651, 0.276393, 0.447214, 0.57735, 0.794655, 0.187592,
	0.525731, 0.723607, -0.447214, 0.57735, 0.794655, 0.187592,
	-1.45721e-07, 0.894427, 0.447214, 0.57735, 0.794655, 0.187592,
	-1.45721e-07, 0.894427, 0.447214, 0, 0.982247, -0.187592,
	0.525731, 0.723607, -0.447214, 0, 0.982247, -0.187592,
	-0.525731, 0.723607, -0.447214, 0, 0.982247, -0.187592,
	0.525731, 0.723607, -0.447214, 0, 0.607062, -0.794654,
	0, 0, -1, 0, 0.607062, -0.794654,
	-0.525731, 0.723607, -0.447214, 0, 0.607062, -0.794654,
	0, 0, 1, -0.356822, 0.491123, 0.794654,
	-1.45721e-07, 0.894427, 0.447214, -0.356822, 0.491123, 0.794654,
	-0.850651, 0.276393, 0.447214, -0.356822, 0.491123, 0.794654,
	-1.45721e-07, 0.894427, 0.447214, -0.57735, 0.794654, 0.187592,
	-0.525731, 0.723607, -0.447214, -0.57735, 0.794654, 0.187592,
	-0.850651, 0.276393, 0.447214, -0.57735, 0.794654, 0.187592,
	-0.850651, 0.276393, 0.447214, -0.934172, 0.303531, -0.187592, 
	-0.525731, 0.723607, -0.447214, -0.934172, 0.303531, -0.187592,
	-0.850651, -0.276393, -0.447214, -0.934172, 0.303531, -0.187592,
	-0.525731, 0.723607, -0.447214, -0.57735, 0.187593, -0.794654,
	0, 0, -1, -0.57735, 0.187593, -0.794654,
	-0.850651, -0.276393, -0.447214, -0.57735, 0.187593, -0.794654,
	0, 0, 1, -0.57735, -0.187593, 0.794654,
	-0.850651, 0.276393, 0.447214, -0.57735, -0.187593, 0.794654,
	-0.525731, -0.723607, 0.447214, -0.57735, -0.187593, 0.794654,
	-0.850651, 0.276393, 0.447214, -0.934172, -0.303531, 0.187593,
	-0.850651, -0.276393, -0.447214, -0.934172, -0.303531, 0.187593,
	-0.525731, -0.723607, 0.447214, -0.934172, -0.303531, 0.187593,
	-0.525731, -0.723607, 0.447214, -0.57735, -0.794654, -0.187592,
	-0.850651, -0.276393, -0.447214, -0.57735, -0.794654, -0.187592,
	-3.90967e-08, -0.894427, -0.447214, -0.57735, -0.794654, -0.187592,
	-0.850651, -0.276393, -0.447214, -0.356822, -0.491123, -0.794654,
	0, 0, -1, -0.356822, -0.491123, -0.794654,
	-3.90967e-08, -0.894427, -0.447214, -0.356822, -0.491123, -0.794654
};

unsigned int sphereindices[] = {
	0, 1, 2, 
	3, 4, 5,
	6, 7, 8,
	9, 10, 11,
	12, 13, 14,
	15, 16, 17,
	18, 19, 20,
	21, 22, 23,
	24, 25, 26,
	27, 28, 29,
	30, 31, 32,
	33, 34, 35,
	36, 37, 38,
	39, 40, 41,
	42, 43, 44,
	45, 46, 47,
	48, 49, 50,
	51, 52, 53,
	54, 55, 56,
	57, 58, 59
};




int main()
{
	// Initialize GLFW
	glfwInit();

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

	// Specify the viewport of OpenGL in the Window. From x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 1.5f);
	// glm::mat4 lightModel = glm::mat4(1.0f);
	// lightModel = glm::translate(lightModel, lightPos);

	glm::mat4 planeModel = glm::mat4(1.0f);

	// Generates Shader
	// Shader shaderProgram("../Shaders/default.vert", "../Shaders/default.frag");
	Shader planeProgram("../Shaders/plane.vert", "../Shaders/plane.frag");
	Shader sphereProgram("../Shaders/sphere.vert", "../Shaders/sphere.frag");

	// shaderProgram.Activate();
	// glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	// glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	planeProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(planeProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniform4f(glGetUniformLocation(planeProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(planeProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	sphereProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniform4f(glGetUniformLocation(sphereProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(sphereProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

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



	// Plane
	VAO pVAO;
	pVAO.Bind();
	VBO pVBO(vertices, sizeof(vertices));
	EBO pEBO(indices, sizeof(indices));
	pVAO.LinkAttrib(pVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)(0 * sizeof(float)));
	pVAO.LinkAttrib(pVBO, 1, 2, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	pVAO.LinkAttrib(pVBO, 2, 3, GL_FLOAT, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	pVAO.Unbind();
	pVBO.Unbind();
	pEBO.Unbind();	
	Texture texture("../Textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.texUnit(planeProgram, "tex0", 0);
	Texture spec("../Textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	texture.texUnit(planeProgram, "tex1", 1);


	// Sphere
	Icosphere sphere(0.0f, 0.0f, 0.0f, 1.0f);	
	VAO sVAO;
	sVAO.Bind();
	// VBO sVBO(spherevertices, sizeof(spherevertices));
	VBO sVBO(sphere.interleavedVertices);
	// EBO sEBO(sphereindices, sizeof(sphereindices));
	EBO sEBO(sphere.indices);
	sVAO.LinkAttrib(sVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *)(0 * sizeof(float)));
	sVAO.LinkAttrib(sVBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	sVAO.Unbind();
	sVBO.Unbind();
	sEBO.Unbind();

	// for (int i = 0; i < (int)sphere.vertices.size(); i) {
    //     std::cout << sphere.vertices[i++] << " " <<  sphere.vertices[i++] << " " << sphere.vertices[i++] << " " << std::endl;
    //     // std::cout << sphere.indices[i++] << " " <<  sphere.indices[i++] << " " << sphere.indices[i++] << " " << std::endl;
    //     if(i%9==0) std::cout << std::endl;
    // }
	
	// for (int i = 0; i < (int)sphere.interleavedVertices.size(); i) {
    //     std::cout << sphere.interleavedVertices[i++] << ", " <<  sphere.interleavedVertices[i++] << ", " << sphere.interleavedVertices[i++] << ", ";
    //     std::cout << sphere.interleavedVertices[i++] << ", " <<  sphere.interleavedVertices[i++] << ", " << sphere.interleavedVertices[i++] << ", " << std::endl;
    //     // std::cout << sphere.indices[i++] << " " <<  sphere.indices[i++] << " " << sphere.indices[i++] << " " << std::endl;
    //     // if(i%6==0) std::cout << std::endl;
    // }

	// for ( int i = 0; i < (int)sphere.indices.size(); i) {
    //     std::cout << sphere.indices[i++] << ", " <<  sphere.indices[i++] << ", " << sphere.indices[i++] << ", " << std::endl;
	// }

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
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);
			// setting cam

		// Draw a model
		// shaderProgram.Activate();
		// treeModel.Draw(shaderProgram, camera);

		planeProgram.Activate();
		glUniform3f(glGetUniformLocation(planeProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(planeProgram, "camMatrix"); // give camMatrix to shader
		pVAO.Bind();
		texture.Bind();
		spec.Bind();
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		sphereProgram.Activate();
		glUniform3f(glGetUniformLocation(sphereProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(sphereProgram, "camMatrix"); // give camMatrix to shader
		sVAO.Bind();
		glDrawElements(GL_TRIANGLES, (int)sphere.indices.size(), GL_UNSIGNED_INT, 0);


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	pVAO.Delete();
	pVBO.Delete();
	pEBO.Delete();
	sVAO.Delete();
	sVBO.Delete();
	sEBO.Delete();
	texture.Delete();
	spec.Delete();

	// shaderProgram.Delete();
	planeProgram.Delete();
	sphereProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}