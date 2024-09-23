#include <iostream>

#include "SDL_context.h"

SDL::SDL_context::SDL_context() {
    sdl_window = NULL;
    sdl_renderer = NULL;
}

bool SDL::SDL_context::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    sdl_window = SDL_CreateWindow("MPI_Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (sdl_window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sdl_renderer == nullptr) {
        SDL_DestroyWindow(sdl_window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}

void SDL::SDL_context::endSDL() {
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void SDL::SDL_context::onEvent(SDL_Event* event) {
    if (event->type == SDL_QUIT) {
        stop_flag = true;
    }
}

bool SDL::SDL_context::getStopSDL() { return stop_flag; }

void SDL::SDL_context::eventHandling() {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        onEvent(&event);
    }
}