#ifndef APPLICATION_H
#define APPLICATION_H

#include "../context/SDL_context.h"
#include "../context/MPI_context.h"
#include "scene.h"

class Application {

private:
    SDL::SDL_context* sdl_ctx;
    MPI::MPI_context* mpi_ctx;

    bool isRunning;

public:
    Application(int argc, char *argv[]);
    ~Application();

    void initScene();

    void execute();
};

#endif