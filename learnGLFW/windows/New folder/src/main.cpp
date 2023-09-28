#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

// Specify the coordinate of vertices
/*
    x and y origin is located in the middle of the window
    most left of the window is -1 and most right is 1
    y axis same concept
*/

int main() 
{
    if(!glfwInit()) 
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = 
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f
    };


    GLFWwindow *window = glfwCreateWindow(800, 800, "Triangle", NULL, NULL);

    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW windwo" << std::endl;
        return -2;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, 800, 800);


    // shader is an OpenGL object, in the background
    // to access by referrences

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // glCreateShader is used to create shade (shader kind)

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        //        (reference value, 1 string, source code (shader), ) 	
        // (GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
        // fit glsl source code to the shader reference value
    
    glCompileShader(vertexShader);
        // compile imediately after assingning the source

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // To use shader, we have to wrap it in shader program
    GLuint shaderProgam = glCreateProgram();
    
    //  Attach the shader to the program 
    glAttachShader(shaderProgam, vertexShader);
    glAttachShader(shaderProgam, fragmentShader);

    // Wrap shaderProgram
    glLinkProgram(shaderProgam);

    // because already in the program it is okay to delete
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    /*
        sending data between cpu and gpu is kind of slow 
        so you have to send it in a big batch (buffer)
    */

    // Vertex Buffer Object, to store vertex data
    GLuint VAO, VBO;
        // VBO
        // actually is an array of references
        // since we only have one object, we only need one

    // should generate befor VBO
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
        // (n, *buffers)
        // generate buffer object names
        // returns n buffer object names in buffers.


    /*
        binding -> make a cetain object the current object
        whenever we fire a function that would modify that type of object it modifies the current object (binded object)
    */
    glBindVertexArray(VAO);
        // bind a vertex array object

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // type reference : https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml    
        // GL_ARRAY_BUFFER is used becuse that's the type we need to use for the vertex buffer
        // bind a named buffer object
        // binds a buffer object to the specified buffer binding point.

    // store vertices in VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // (GLenum target, GLsizeiptr size (in bytes), const void * data, GLenum usage)
        //  creates and initializes a buffer object's data store
        //  create a new data store for a buffer object.
        // GL_STREAM -> the vertices will be modified once and used a few times
        // GL_STATIC -> the vertices will be modified once and used many times
        // GL_DYNAMIC -> the vertices will be modified many times and used many times
        // _DRAW -> the vertices will be modified and used to draw an image in the screen
        // _COPY -> the vertices will be modified and used to copy an image in the screen
        // _READ -> the vertices will be modified and used to read an image in the screen

    // OpenGL doesn't know where to find the packed object of vetex data
    // (VAO) Vertex array will store pointer to one or move vbo and tell OpenGL how to interprate them
    // initialized on top near VBO

    /*configure so that OpenGL know how to use VAO*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        // (position of vertex attribute, value per vertex (3 floats), data type, only matter if coordinate in int,
        // stride (ammount of data between each vertex), offset (pointer where the vertices begin))
        // define an array of generic vertex attribute data
        // specify the location and data format of the array of generic vertex attributes at index. 
        // index to use when rendering.

    glEnableVertexAttribArray(0);
        // Enable or disable a generic vertex attribute array
    
    /*So that we won't accidentally change a VBA or VBO with a function*/
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
        // By binding to 0, means we combinge VAO and VBO 

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgam);
            // Activate the shader program
        
        glBindVertexArray(VAO);
            // Bind the VAO (Tell OpenGL we want to use this one)
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
            // (mode, starting index, ammount of vertices)
            // Drawing function
        
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgam);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}