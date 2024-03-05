#include <iostream>

#include "application.h"

Application::Application(int argc, char *argv[]) {
    isRunning = true;

    mpi_ctx = MPI_context(argc, argv);

    if(mpi_ctx.getRank() == 0) {
        sdl_ctx = SDL_context();

        execute();
    } 
}

int Application::execute() {
    while(isRunning) {
        sdl_ctx.eventHandling();

        std::cout << "test" << std::endl;
        //render

    }
    exit();
    return 0;
}

void Application::exit() {
    //if(sdl_ctx) sdl_ctx.endSDL();
    mpi_ctx.endMPI();
    isRunning = false;
}