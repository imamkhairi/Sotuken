#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() 
{
    // Initialize GLFW
    if (!glfwInit()) return -1;

    // Tell GLFW what version of OpenGL we are using
    // In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        // tell glfw what version of opengl will be used
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // opengl profile sort of a package of function
        // core -> modern
        // compatibility compatible for modern and outdated

    // Create a GLFWwindow object
    GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
        //                                (widht, height, title, fullscreen, ?)
    
    // Error check if the window failed to create
    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Introduce the window into the current context
    glfwMakeContextCurrent(window);
        // Makes the context of the specified window current for the calling thread.
        // tell glfw that window is part of current context
        // context -> (not excact) object that hold the whole of opengl

    // Load GLAD so it configures OpenGL
    gladLoadGL();
        // load needed configuration to opengl
    
    // Specify the viewport of OpenGL in the Window
    // In this case it goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, 800, 800);
        // set which area we want opengl to rendering
    
    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // bottom left to top right
    
    // Clean the back buffer and assign the new color to it 
    glClear(GL_COLOR_BUFFER_BIT);
        // tell opengl to execute the command we have told it to prepare 
    
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    // the main while loop
    while(!glfwWindowShouldClose(window))
    {
        // Take care of all GLFW events
        glfwPollEvents();
            // tell glfw to process all the poll event (window appearing, resize, etc)
            // if don't do this, the window will always in not responding state
    }

    // Delete window before ending the program
    glfwDestroyWindow(window);

    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}