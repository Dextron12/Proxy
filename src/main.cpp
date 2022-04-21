#include <iostream>
#include <direct.h>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.hpp"
#include "Camera.hpp"


GLFWwindow* window;
int width = 800;
int height = 600;

std::string baseDir = _getcwd(NULL, 0);

float vertices[] = {
    //Draw vertices counter-clockwise
    0.5f, -0.5f, 0.0f,
    0.0f, -0.5f, 1.0f,
    -0.5f, -0.5f, 0.0f
};


void framebufferCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float deltaTime);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);



Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));


int main(){
    glfwInit();

    window = glfwCreateWindow(width, height, "Proxy", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Faield to initate GLAD" << std::endl;
        return -1;
    }

    //Set callbacks here <---
    glfwSetFramebufferSizeCallback(window, framebufferCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    //Set Mouse Input mode Here <----
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    unsigned int VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    Shader shader(baseDir + "\\Shaders\\shader.vs", baseDir + "\\Shaders\\shader.fs");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    shader.use();
    shader.setMat4("projection", projection);
    glm::mat4 view;

    float NOW, LAST, deltaTime;
    NOW = glfwGetTime();
    LAST = 0; deltaTime = 0;

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        glBindVertexArray(VAO);
        /*glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 3);*/


        for (int i = 1; i <= 10; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f+i, 0.0f, 0.0f));
            if (i % 2){
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.0f));
            }
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        processInput(window, deltaTime);


        glfwSwapBuffers(window);
        glfwPollEvents();

        LAST = NOW;
        NOW = glfwGetTime();
        deltaTime = (NOW - LAST);
    }
    glfwTerminate();
    return 0;
}


void framebufferCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    width = width;
    height = height;
}

void processInput(GLFWwindow* window, float deltaTime){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.updateKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.updateKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.updateKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.updateKeyboard(RIGHT, deltaTime);
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos){
    camera.updateMouse(xpos, ypos);
}