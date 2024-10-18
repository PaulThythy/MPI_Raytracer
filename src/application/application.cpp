#include <iostream>
#include <thread>
#include <atomic>
#include <memory>

#ifdef _WIN32
	#include <SDL.h>
#elif __linux__
	#include <SDL2/SDL.h>
#endif

#include "application.h"

Application::Application(int argc, char *argv[]) {
    m_isRunning = true;

    m_mpiCtx = new MPI::MPI_context(argc, argv);
    m_sdlCtx = new SDL::SDL_context();

    m_scene = std::make_unique<Scene>();

    if (m_mpiCtx->getRank() == 0) {
        if (!m_sdlCtx->initSDL()) {
            m_isRunning = false;
        }
    }

    std::thread eventThread;
    if (m_mpiCtx->getRank() == 0) {
        eventThread = std::thread(&Application::eventListener, this);
    }

    m_scene->render(m_mpiCtx, m_sdlCtx, m_isRunning);

    if (m_mpiCtx->getRank() == 0 && eventThread.joinable()) {
        eventThread.join();
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

void Application::eventListener() {
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
                //TODO add camera movement features
                if (event.key.keysym.sym == SDLK_z) { 
                    std::cout << "Moving camera forward...\n";
                    //m_scene->m_camera.m_lookFrom.z -= 5.0f;  
                    //m_scene->m_camera.updateCamera();  
                    //SDL_RenderClear(m_sdlCtx->getRenderer());     
                    //m_scene->render(m_mpiCtx, m_sdlCtx, m_isRunning);
                }
                if (event.key.keysym.sym == SDLK_s) { 
                    std::cout << "Moving camera backward...\n";
                    //m_scene->m_camera.m_lookFrom.z += 5.0f;  
                    //m_scene->m_camera.updateCamera();  
                    //SDL_RenderClear(m_sdlCtx->getRenderer()); 
                    //m_scene->render(m_mpiCtx, m_sdlCtx, m_isRunning);
                }
                if (event.key.keysym.sym == SDLK_d) { 
                    std::cout << "Moving camera to the right...\n";
                }
                if (event.key.keysym.sym == SDLK_q) { 
                    std::cout << "Moving camera to the left...\n";
                }
            }
            
            //TODO add events (for moving camera for example)
        }

        //do stuff

        //SDL_Delay(10);
    }
}