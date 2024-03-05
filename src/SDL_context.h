#ifndef SDL_CONTEXT_H
#define SDL_CONTEXT_H

#ifdef _WIN32
    #include <SDL.h>
#elif __linux__
    #include <SDL2/SDL.h>
#endif

class SDL_context {

private:
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    bool stop_flag = false;

public:
    SDL_context();

    bool initSDL();
    void endSDL();

    void onEvent(SDL_Event* event);

    void eventHandling();

    bool getStopSDL();
};

#endif