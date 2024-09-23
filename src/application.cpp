#include <iostream>

#include "application.h"

Application::Application(int argc, char *argv[]) {
    isRunning = true;

    mpi_ctx = new MPI::MPI_context(argc, argv);

    if(mpi_ctx->getRank() == 0) {
        sdl_ctx = new SDL::SDL_context();

        if (sdl_ctx->initSDL()) {
            execute();
        } else {
            isRunning = false;
        }
    }
}

Application::~Application() {
    if (mpi_ctx != NULL) {
        mpi_ctx->endMPI();
        delete mpi_ctx;
    }
    
    if (sdl_ctx != NULL) {
        sdl_ctx->endSDL();
        delete sdl_ctx;
    }
}

void Application::execute() {
    while(isRunning) {
        //sdl_ctx->eventHandling();

        /* if (sdl_ctx->getStopSDL()) {
            isRunning = false;
        } */

        //std::cout << "I'm the rank : " << mpi_ctx->getRank() << std::endl;
        //render

    }
}