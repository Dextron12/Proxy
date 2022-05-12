
#include "Window.hpp"

t_Window::t_Window(const char* windowTitle, int windowWidth, int windowHeight) : width(windowWidth), height(windowHeight) {
    //Initate SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "Failed ot initate SDL2" << std::endl;
        b_appState = false;
    }
    //Create window & context
    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cout << "Faield to create a window context" << std::endl;
        b_appState = false;
    }
    context = SDL_GL_CreateContext(window);
    //Init GLAD
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initate GLAD" << std::endl;
        b_appState = false;
    }
}

void t_Window::updateEvents(){
    while (SDL_PollEvent(&event)){
        
        if (event.type == SDL_QUIT){
            b_appState = false;
        }

        if (event.type == SDL_WINDOWEVENT) {
            if (event.window.type == SDL_WINDOWEVENT_RESIZED){
                SDL_GetWindowSize(window, &width, &height);
                glViewport(0, 0, width, height);
            }
        }
    }
}

void t_Window::destroyContext(){
    SDL_GL_DeleteContext(context);
}