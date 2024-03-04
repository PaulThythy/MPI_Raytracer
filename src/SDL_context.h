#ifndef SDL_CONTEXT_H
#define SDL_CONTEXT_H

class SDL_context {

private:
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

public:
    SDL_context();
    ~SDL_context();

    void initSDL();
    void endSDL();
};

#endif