#include <iostream>

#include <memory>

#include "application.h"
#include "scene.h"
#include "../math/sphere.h"
#include "../globals/globals.h"
#include "../random/random.h"

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
    int image_width = Config::WINDOW_WIDTH;
    int image_height = Config::WINDOW_HEIGHT;

    std::cout << image_width << "x" << image_height << std::endl;

    glm::vec3 lookFrom(13., 2., 3.);
    glm::vec3 lookAt(0.0, 0.0, 0.0);
    glm::vec3 up(0.0, 1.0, 0.0);
    double vfov = 90.0;                                                 // Vertical field of view in degrees
    double aspectRatio = double(image_width) / double(image_height);
    double aperture = 0.0;                                              // No depth of field for simplicity
    double focusDist = 1.0;                                             // Focusing distance
    Camera::Camera cam(lookFrom, lookAt, up, vfov, aspectRatio, aperture, focusDist);
    Scene::Scene scene(cam);
    auto sphere1 = std::make_shared<Sphere::Sphere>(glm::vec3(0., 0., 0.), 2.0);
    auto sphere2 = std::make_shared<Sphere::Sphere>(glm::vec3(2.1, 2.1, 2.1), 0.5);
    scene.addObject(sphere1);
    scene.addObject(sphere2);

    // ray tracing loops
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {

            for (int s = 0; s < Config::SAMPLES; ++s) {
                // Normalize x, y coordinates in the scene
                double u = (i + Random::randomDouble()) / (image_width - 1);
                double v = (j + Random::randomDouble()) / (image_height - 1);
                Ray::Ray ray = cam.getRay(u, v);

                float t;

                if (sphere1->intersect(ray, t)) {
                    sdl_ctx->setPixel(i, j, 255, 0, 0);
                } else if(sphere2->intersect(ray, t)) {
                    sdl_ctx->setPixel(i, j, 0, 0, 255);
                } else {
                    sdl_ctx->setPixel(i, j, 0, 0, 0);
                }
            }
        }
    }

    // update screen
    sdl_ctx->updateScreen();

    while(isRunning) {
        //sdl_ctx->eventHandling();

        /* if (sdl_ctx->getStopSDL()) {
            isRunning = false;
        } */ 

        //std::cout << "I'm the rank : " << mpi_ctx->getRank() << std::endl;
        //render


    }
}