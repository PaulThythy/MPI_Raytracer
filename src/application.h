#ifndef APPLICATION_H
#define APPLICATION_H

extern int G_WINDOW_WIDTH;
extern int G_WINDOW_HEIGHT;

#include "SDL_context.h"
#include "MPI_context.h"

class Application {

private:
    SDL_context sdl_ctx;
    MPI_context mpi_ctx;

    //Renderer* renderer;

    bool isRunning;

public:
    Application(int argc, char *argv[]);

    int execute();
    void exit();
};

#endif