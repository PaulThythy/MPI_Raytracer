#include <iostream>

#include <memory>

#include "application.h"
#include "scene.h"
#include "../math/sphere.h"

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
    glm::vec3 lookFrom(13., 2., 3.);
    glm::vec3 lookAt(0., 0., 0.);
    glm::vec3 up(0., 1., 0.);
    Camera::Camera cam(lookFrom, lookAt, up);
    Scene::Scene scene(cam);
    scene.addObject(std::make_shared<Sphere::Sphere>(glm::vec3(0., 0., 0.), 1.0));

    while(isRunning) {
        //sdl_ctx->eventHandling();

        /* if (sdl_ctx->getStopSDL()) {
            isRunning = false;
        } */ 

        //std::cout << "I'm the rank : " << mpi_ctx->getRank() << std::endl;
        //render


    }
}