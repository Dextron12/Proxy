#include <iostream>
#include <direct.h>
#include <math.h>


#include <glad\glad.h>
#include <glfw\glfw3.h>

#include "Shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float vertices[] = {
    -0.5f, -0.5f, 0.0f,     //bottom left
    0.5f, -0.5f, 0.0f,      //bottom right
    0.0f, 0.5f, 0.0f        //middle
};
  

int main(int argc, char* argv[]){
    glfwInit();



    GLFWwindow* window = glfwCreateWindow(800, 600, "Proxy", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initiate GLAD" << std::endl;
        return -1;
    }

    //Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //VBO
    unsigned int VBO, VAO;
    glGenBuffers (1, &VBO);
    glGenVertexArrays(1, &VAO); //VAO

    glBindVertexArray(VAO); // VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::string baseDir = _getcwd(NULL, NULL);
    std::string vShader = "/Shaders/shader.vs";

    Shader shaderProgram(baseDir + "/Shaders/shader.vs", baseDir + "/Shaders/shader.fs");


    //main loop
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);


        //Change colour
        shaderProgram.use();
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        shaderProgram.setVec4("ourColour", 0.0f, greenValue, 0.0f, 1.0f);
        shaderProgram.setFloat("xOffset", 0.5f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}