#include <iostream>

#include <memory>

#include "application.h"

Application::Application(int argc, char *argv[]) {
    m_isRunning = true;

    m_mpiCtx = new MPI::MPI_context(argc, argv);
    m_sdlCtx = new SDL::SDL_context();

    m_scene = std::make_unique<Scene>();

    //TODO modify how render is called, and move the execute function in the render function
    if(m_mpiCtx->getRank() == 0) {
        if (m_sdlCtx->initSDL()) {
            m_scene->render(m_mpiCtx, m_sdlCtx);
            execute();
        } else {
            m_isRunning = false;
        }
    } else {
        m_scene->render(m_mpiCtx, m_sdlCtx);
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
    SDL_Event event;
    while(m_isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { 
                std::cout << "SDL_QUIT event detected. Exiting...\n" << std::endl;
                m_isRunning = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) { 
                    std::cout << "ESCAPE key pressed. Exiting...\n" << std::endl;
                    m_isRunning = false;
                }
                //TODO add camera movement features
                if (event.key.keysym.sym == SDLK_z) { 
                    std::cout << "Moving camera forward...\n" << std::endl;
                }
                if (event.key.keysym.sym == SDLK_s) { 
                    std::cout << "Moving camera backward...\n" << std::endl;
                }
                if (event.key.keysym.sym == SDLK_d) { 
                    std::cout << "Moving camera to the right...\n" << std::endl;
                }
                if (event.key.keysym.sym == SDLK_q) { 
                    std::cout << "Moving camera to the left...\n" << std::endl;
                }
            }
            
            //TODO add events (for moving camera for example)
        }

        //do stuff

        //SDL_Delay(10);
    }
}