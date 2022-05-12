

#pragma once

#include <iostream>
#include <direct.h>
#include <string>

#include <SDL2\SDL.h>
#include <glad\glad.h>

class t_Window{
    private:
        SDL_Event event;
        SDL_GLContext context;

    public:

        int width, height;
        SDL_Window* window;

        bool b_appState = true;

        t_Window(const char* windowTitle, int windowWidth, int windowHeight);

        void updateEvents();

        void destroyContext();

};