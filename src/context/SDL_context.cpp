#include <iostream>

#include "SDL_context.h"

SDL::SDL_context::SDL_context() {
    m_SDLwindow = NULL;
    m_SDLrenderer = NULL;
}

bool SDL::SDL_context::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_SDLwindow = SDL_CreateWindow("MPI_Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (m_SDLwindow == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    m_SDLrenderer = SDL_CreateRenderer(m_SDLwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_SDLrenderer == nullptr) {
        SDL_DestroyWindow(m_SDLwindow);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}

void SDL::SDL_context::endSDL() {
    SDL_DestroyRenderer(m_SDLrenderer);
    SDL_DestroyWindow(m_SDLwindow);
    SDL_Quit();
}

void SDL::SDL_context::setPixel(int x, int y, int r, int g, int b) {
    SDL_SetRenderDrawColor(m_SDLrenderer, r, g, b, 1);
    SDL_RenderDrawPoint(m_SDLrenderer, x, y);
}

void SDL::SDL_context::updateScreen() {
    SDL_RenderPresent(m_SDLrenderer);
}

SDL_Renderer* SDL::SDL_context::getRenderer() { return m_SDLrenderer; }
SDL_Window* SDL::SDL_context::getWindow() { return m_SDLwindow; }