
#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>

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

Camera camera(glm::vec3(67.0f, 627.5f, 169.9f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-64.2f, -128.1f, -42.4f));

std::string dirPath;


void processInput();
void processMouse();

bool firstMouse = true;

int mouseX, mouseY;
int lastX, lastY;




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

    /*SDL_CaptureMouse(SDL_TRUE); // Alows global mouse capture
    SDL_ShowCursor(SDL_DISABLE);
    SDL_WarpMouseInWindow(window.window, 0, 0);*/

    SDL_SetWindowMouseGrab(window.window, SDL_TRUE);
    SDL_CaptureMouse(SDL_TRUE);

    glEnable(GL_DEPTH_TEST);

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
    
    


    int width, height, nrChannels;
    std::string imgPath = dirPath + "/Textures/iceland_heightmap.png";
    unsigned char *data = stbi_load(imgPath.c_str(), &width, &height, &nrChannels, 0);
    if (!data){
        std::cout << "Failed to load height map" << std::endl;
    }

    //vertex generation
    std::vector<float> vertices;
    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    for (unsigned int i = 0; i < height; i++){
        for (unsigned int j = 0; j < width; j++){
            //retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + width * i) * nrChannels;
            unsigned char y = texel[0];

            //vertex
            vertices.push_back(-height/2.0f + i);
            vertices.push_back((int)y * yScale - yShift);
            vertices.push_back(-width/2.0f + j);
        }
    }
    //Free height map from mem
    stbi_image_free(data);

    //Calculate indicies (for EBO)
    std::vector<unsigned int> indicies;
    for (unsigned int i = 0; i < height-1; i++){
        for (unsigned int j = 0; j < width; j++){
            for (unsigned int k = 0; k < 2; k++){
                indicies.push_back(j + width * (i + k));
            }
        }
    }

    const unsigned int NUM_STRIPS = height-1;
    const unsigned int NUM_VERTS_PER_STRIP = width*2;

    unsigned int terrainVAO, terrainVBO, terrainEBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    //Position attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

    //deltaTime
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;




    while (window.b_appState){
        float currentFrame = SDL_GetPerformanceCounter();
        deltaTime = (double)((currentFrame - lastFrame)*1000 / (double)SDL_GetPerformanceFrequency() );
        lastFrame = currentFrame;
        window.updateEvents();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        yellowTriangle.use();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        yellowTriangle.setMat4("projection", projection);
        yellowTriangle.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        yellowTriangle.setMat4("model", model);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(terrainVAO);
        for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip){
            glDrawElements(GL_TRIANGLE_STRIP, (NUM_VERTS_PER_STRIP), GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * NUM_VERTS_PER_STRIP * strip));
        }

        processMouse();

        auto key = SDL_GetKeyboardState(NULL);

        if (key[SDL_SCANCODE_W]){
            camera.updateKeyboard(FORWARD, deltaTime);
        }

        if (key[SDL_SCANCODE_S]){
            camera.updateKeyboard(BACKWARD, deltaTime);
        }

        if (key[SDL_SCANCODE_A]){
            camera.updateKeyboard(LEFT, deltaTime);
        }

        if (key[SDL_SCANCODE_D]){
            camera.updateKeyboard(RIGHT, deltaTime);
        }



        SDL_GL_SwapWindow(window.window);
    }
    window.destroyContext();
    SDL_DestroyWindow(window.window);
    return 0;
}

void processMouse(){
    SDL_GetMouseState(&mouseX, &mouseY);
    if (firstMouse){
        lastX = mouseX;
        lastY = mouseY;
        firstMouse = false;
    }

    float xoffset = mouseX - lastX;
    float yoffset = lastY - mouseY;

    lastX = mouseX;
    lastY = mouseY;

    camera.updateMouse(mouseX, mouseY);
}