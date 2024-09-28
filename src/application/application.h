#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "context/SDL_context.h"
#include "context/MPI_context.h"
#include "scene.h"

class Application {

private:
    SDL::SDL_context* m_sdlCtx;
    MPI::MPI_context* m_mpiCtx;

    bool m_isRunning;

    std::unique_ptr<Scene> m_scene;

public:
    Application(int argc, char *argv[]);
    ~Application();

    void initScene();

    void execute();
};

#endif