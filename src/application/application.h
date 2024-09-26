#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "context/SDL_context.h"
#include "context/MPI_context.h"
#include "scene.h"

class Application {

private:
    SDL::SDL_context* sdl_ctx;
    MPI::MPI_context* mpi_ctx;

    bool isRunning;

    std::unique_ptr<Scene> m_scene;

public:
    Application(int argc, char *argv[]);
    ~Application();

    void initScene();

    void execute();
};

#endif