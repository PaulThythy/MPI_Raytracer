#include <iostream>

#include <memory>

#include "application.h"

Application::Application(int argc, char *argv[]) {
    m_isRunning = true;

    m_mpiCtx = new MPI::MPI_context(argc, argv);
    m_sdlCtx = new SDL::SDL_context();

    m_scene = std::make_unique<Scene>();

    if(m_mpiCtx->getRank() == 0) {
        if (m_sdlCtx->initSDL()) {
            execute();
        } else {
            m_isRunning = false;
        }
    }
}

Application::~Application() {
    if (m_mpiCtx != NULL) {
        m_mpiCtx->endMPI();
        delete m_mpiCtx;
    }
    
    if (m_sdlCtx != NULL) {
        m_sdlCtx->endSDL();
        delete m_sdlCtx;
    }
}

void Application::execute() {
    m_scene->render(m_sdlCtx);

    SDL_Event event;
    while(m_isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { 
                std::cout << "SDL_QUIT event detected. Exiting...\n";
                m_isRunning = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) { 
                    std::cout << "ESCAPE key pressed. Exiting...\n";
                    m_isRunning = false;
                }
            }

            //TODO add events (for moving camera for example)
        }

        //do stuff

        //SDL_Delay(10);
    }
}