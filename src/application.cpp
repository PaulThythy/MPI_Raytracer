#include <iostream>

#include "application.h"

Application::Application(int argc, char *argv[]) {
    isRunning = true;

    mpi_ctx = new MPI_context(argc, argv);

    if(mpi_ctx->getRank() == 0) {
        //sdl_ctx = new SDL_context();

        execute();
    }
}

Application::~Application() {

}

void Application::execute() {
    while(isRunning) {
        //sdl_ctx->eventHandling();

        std::cout << "I'm the rank : " << mpi_ctx->getRank() << std::endl;
        //render

    }
    exit();
}

void Application::exit() {
    //if(sdl_ctx != NULL) sdl_ctx->endSDL();
    mpi_ctx->endMPI();
    isRunning = false;
}