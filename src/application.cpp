#include "application.h"

Application::Application(int argc, char *argv[]) {
    isRunning = true;

    init(argc, argv);
}

void Application::init(int argc, char *argv[]) {
    mpi_ctx = MPI_context(argc, argv);

    if(mpi_ctx.getRank() == 0) {
        sdl_ctx = SDL_context();

        execute();
    }
}

int Application::execute() {
    while(isRunning) {
        //while SDLEventHandling

        //render

    }
    exit();
    return 0;
}

void Application::exit() {
    if(sdl_ctx) sdl_ctx.endSDL();
    mpi_ctx.endMPI();
    isRunning = false;
}