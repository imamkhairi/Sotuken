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

    // Vertices coordinates
    // Equilateral Triangle
    // GLfloat vertices[] = 
    // {
    //     -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
    //      0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
    //      0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
    //     -0.5f / 2,  0.5f * float(sqrt(3)) / 6, 0.0f, // inner left
    //     0.5f / 2 ,  0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
    //     0.0f,  -0.5f * float(sqrt(3)) / 3, 0.0f, // Inner down
    // };

    
    // Exercise
    GLfloat vertices[] = {
        -0.5f, -0.5f, // bottom left
        0.5f, -0.5f, // bottom right
        -0.5f, 0.5f, // upper left
        0.5f, 0.5f, // upper right
    };

    // GLuint indices[] = {
    //     0, 3, 5, // Lower left triangle
    //     3, 2, 4, // Lower Right triangle
    //     5, 4, 1 // Upper triangle
    // };

    // Exercise 
    GLuint indices[] = {
        0, 1, 3,
        3, 2, 0
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

    // Create Vertex Shader Object and get reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // glCreateShader is used to create shade (shader kind)
    // Attach vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        //        (reference value, 1 string, source code (shader), ) 	
        // (GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
        // fit glsl source code to the shader reference value
    // Conpile the Vertex Shader into machine code
    glCompileShader(vertexShader);
        // compile imediately after assingning the source

    // Create Fragment Shader Object and get its reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compile the Fragment Shader ito machine code
    glCompileShader(fragmentShader);


    // To use shader, we have to wrap it in shader program

    // Create Shader Program Object and get its reference
    GLuint shaderProgam = glCreateProgram();
    
    //  Attach the shader to the program 
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(shaderProgam, vertexShader);
    glAttachShader(shaderProgam, fragmentShader);

    // Wrap shaderProgram
    // Wrap-up/link all the shaders together into the Shader Program
    glLinkProgram(shaderProgam);

    // because already in the program it is okay to delete
    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    /*
        sending data between cpu and gpu is kind of slow 
        so you have to send it in a big batch (buffer)
    */

    // Vertex Buffer Object, to store vertex data
    // Create reference containers for the Vertex Array Object and the Vertex Buffer Object
    GLuint VAOs[1], VBOs[1], EBOs[1];
        // actually is an array of references
        // since we only have one object, we only need one

        // EBO -> index buffer tells OpenGL the order in which it should go over vertices

    // should generate befor VBO
    // Generate the VAO and VBO with only 1 object each
    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, VBOs);
        // (n, *buffers)
        // generate buffer object names
        // returns n buffer object names in buffers.
    glGenBuffers(1, EBOs);

    /*
        binding -> make a cetain object the current object
        whenever we fire a function that would modify that type of object it modifies the current object (binded object)
    */

    // Make the VAO the current Vertex Array Object by binding it
    glBindVertexArray(VAOs[0]); //-> karena tadi generate 1 jadinya ada data di [0]
        // bind a vertex array object

    // Bind the VBO specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        // type reference : https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml    
        // GL_ARRAY_BUFFER is used becuse that's the type we need to use for the vertex buffer
        // bind a named buffer object
        // binds a buffer object to the specified buffer binding point.
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);

    // store vertices in VBO
    // Introduce the vertices into the VBO
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
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // OpenGL doesn't know where to find the packed object of vetex data
    // (VAO) Vertex array will store pointer to one or move vbo and tell OpenGL how to interprate them
    // initialized on top near VBO

    /*configure so that OpenGL know how to use VAO*/
    // Configure the Vertex Attribute so that OpenGL knows how to read the VAO
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        // (position of vertex attribute, value per vertex (3 floats), data type, only matter if coordinate in int,
        // stride (ammount of data between each vertex), offset (pointer where the vertices begin))
        // define an array of generic vertex attribute data
        // specify the location and data format of the array of generic vertex attributes at index. 
        // index to use when rendering.

    // exercise
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    // Enable the Vertex Attribute so that OpenGL knows how to use it
    glEnableVertexAttribArray(0);
        // Enable or disable a generic vertex attribute array
    
    /*So that we won't accidentally change a VBA or VBO with a function*/
    // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
        // By binding to 0, means we combinge VAO and VBO 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        // bind after unbinded VAO, before dont want to use the EBO
        // since EBO is stored in VAO

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Tell OpneGL which Shader Program we want to use
        glUseProgram(shaderProgam);
            // Activate the shader program
        
        // Binde the VAO so OpenGL knows to use it
        // glBindVertexArray(VAO);
        glBindVertexArray(VAOs[0]);
            // Bind the VAO (Tell OpenGL we want to use this one)
        
        // Draw the triangle using the GL_TRIANGLES primitive
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // (primitive type, ammount of data, data type of indices, index (start))
            // render primitives from array data
            // Drawing function
        
        glfwSwapBuffers(window);
    }

    // Delete all the objects we've created
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);
    glDeleteBuffers(1, EBOs);
    glDeleteProgram(shaderProgam);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}