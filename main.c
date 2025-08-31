#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <stdbool.h>

//Note: gdi32.lib is a dependency of glfw, so you have to remember to link against it when using opengl
//This goes for either the libglfw3.a or glfw3.lib files (the former being compiled with mingw and the latter being compiled with visual studio)
//gcc main.c "C:\ProgrammingUtils\OpenGL\lib\glad.c" -I"C:\ProgrammingUtils\OpenGL\includes" -L"C:\ProgrammingUtils\OpenGL\lib" -l:libglfw3.a -lgdi32 -o main

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void shaderProgramCreate(unsigned int* shaderProgram, char* vertexPath, char* fragmentPath);

int main(void) {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL!", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    unsigned int shaderProgram;
    shaderProgramCreate(&shaderProgram, "vert.glsl", "frag.glsl");
    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glUseProgram(shaderProgram);
        //glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void shaderProgramCreate(unsigned int* shaderProgram, char* vertexPath, char* fragmentPath) {
    FILE* fptr = fopen(vertexPath, "rb");
    if (fptr == NULL) {
        printf("Could not find vertex shader, was unable to open it, or the file %s does not exist.\n", vertexPath);
        exit(-1);
    }

    fseek(fptr, 0L, SEEK_END);
    int vertexShaderSourceLen = ftell(fptr) / sizeof(char);
    fseek(fptr, 0L, SEEK_SET);
    
    char* vertexShaderSource = (char*)malloc((vertexShaderSourceLen+1) * sizeof(char));
    if (vertexShaderSource == NULL) {
        printf("Failed to allocate memory for vertexShaderSource.\n");
        exit(-1);
    }
    fread(vertexShaderSource, sizeof(char), vertexShaderSourceLen, fptr);
    vertexShaderSource[vertexShaderSourceLen] = '\0';
    fclose(fptr);

    //--------------------------------------------------------------------------

    fptr = fopen(fragmentPath, "rb");
    if (fptr == NULL) {
        printf("Could not find fragment shader, was unable to open it, or the file %s does not exist.\n", fragmentPath);
        exit(-1);
    }

    fseek(fptr, 0L, SEEK_END);
    int fragmentShaderSourceLen = ftell(fptr) / sizeof(char);
    fseek(fptr, 0L, SEEK_SET);
    
    char* fragmentShaderSource = (char*)malloc((fragmentShaderSourceLen+1) * sizeof(char));
    if (fragmentShaderSource == NULL) {
        printf("Failed to allocate memory for fragmentShaderSource.\n");
        exit(-1);
    }
    fread(fragmentShaderSource, sizeof(char), fragmentShaderSourceLen, fptr);
    fragmentShaderSource[fragmentShaderSourceLen] = '\0';
    fclose(fptr);

    //-------------------------------------------------------------------------------

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char* const*)&vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char* const*)&fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, 512, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, vertexShader);
    glAttachShader(*shaderProgram, fragmentShader);
    glLinkProgram(*shaderProgram);

    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free(vertexShaderSource);
    free(fragmentShaderSource);
}