
#include "Window.hpp"
#include "Shader.hpp"

/*float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};*/

float vertices[] = {
    -0.75f, -0.25f, 0.0f,
    -0.25, -0.25, 0.0f,
    -0.5f, 0.25, 0.0f,

    //Second triangle
    0.25f, -0.25f, 0.0f,
    0.75f, -0.25f, 0.0f,
    0.5f, 0.25f, 0.0f
};


unsigned int indicies[] = {
    //First triangle
    0, 1, 3,
    1, 2, 3
};

float triangle1[] = {
    -0.75f, -0.25f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.25f, -0.25f, 0.0f
};
float triangle2[] = {
    0.25f, -0.25f, 0.0f,
    0.75f, -0.25f, 0.0f,
    0.5f, 0.25f, 0.0f
};

std::string dirPath;


int main(int argc, char** argv){
    t_Window window("Proxy", 800, 600);
    dirPath = _getcwd(NULL, 0);

    /*unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO); // Generate EBO buffer

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind EBO buffer

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);*/

    unsigned int triVBO, triVAO;
    glGenBuffers(1, &triVBO);
    glGenVertexArrays(1, &triVAO);

    glBindVertexArray(triVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int triVBO1, triVAO1;
    glGenBuffers(1, &triVBO1);
    glGenVertexArrays(1, &triVAO1);

    glBindVertexArray(triVAO1);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    Shader shader(dirPath + "/Shaders/vertex.s", dirPath + "/Shaders/fragment.s");
    Shader yellowTriangle(dirPath + "/Shaders/vertex.s", dirPath + "/Shaders/triangleYellow.fs");

    while (window.b_appState){
        window.updateEvents();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(triVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        yellowTriangle.use();
        glBindVertexArray(triVAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window.window);
    }
    window.destroyContext();
    SDL_DestroyWindow(window.window);
    return 0;
}