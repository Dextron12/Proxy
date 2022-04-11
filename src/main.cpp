#include <iostream>
#include <direct.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include <glad\glad.h>
#include <glfw\glfw3.h>

#include "Shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadImage(std::string imgPath, GLenum format);

float vertices[] = {
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,    //bottom left
    0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,   1.0f, 0.0f,      //bottom right
    0.0f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,   0.5f, 1.0f         //middle
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    std::string baseDir = _getcwd(NULL, NULL);

    Shader shaderProgram(baseDir + "/Shaders/shader.vs", baseDir + "/Shaders/shader.fs");
    


    loadImage(baseDir + "\\Textures\\wall.jpg", GL_RGB);

    


    //main loop
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        shaderProgram.use();

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

unsigned int loadImage(std::string imgPath, GLenum format){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(imgPath.c_str(), &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Texture: (" << imgPath << ")\nCould not be loaded." << std::endl;
    }
    stbi_image_free(data);
    return texture;
}