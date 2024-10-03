#ifndef SDL_CONTEXT_H
#define SDL_CONTEXT_H

#ifdef _WIN32
	#include <SDL.h>
#elif __linux__
	#include <SDL2/SDL.h>
#endif

#include "globals/globals.h"

namespace SDL {
    class SDL_context {

    private:
        SDL_Window* m_SDLwindow;
        SDL_Renderer* m_SDLrenderer;

    public:
        SDL_context();

        bool initSDL();
        void endSDL();

        void setPixel(int x, int y, int r, int g, int b);
        void updateScreen();
    };
}

#endif