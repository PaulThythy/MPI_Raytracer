#include <iostream>

#include <memory>

#include "application.h"
#include "math/sphere.h"
#include "globals/globals.h"
#include "random/random.h"

Application::Application(int argc, char *argv[]) {
    isRunning = true;

    mpi_ctx = new MPI::MPI_context(argc, argv);

    if(mpi_ctx->getRank() == 0) {
        sdl_ctx = new SDL::SDL_context();

        if (sdl_ctx->initSDL()) {
            initScene();
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

void Application::initScene() {
    glm::vec3 lookFrom(13.0f, 2.0f, 3.0f); 
    glm::vec3 lookAt(0.0f, 0.0f, 0.0f);    
    glm::vec3 up(0.0f, 1.0f, 0.0f);       
    float vfov = 90.0f;                    
    float aspectRatio = static_cast<float>(Config::WINDOW_WIDTH) / static_cast<float>(Config::WINDOW_HEIGHT);
    float aperture = 0.0f;                  
    float focusDist = 1.0f;

    Camera cam(lookFrom, lookAt, up, vfov, aspectRatio, aperture, focusDist);
    m_scene = std::make_unique<Scene>(cam);
}

void Application::execute() {
    int image_width = Config::WINDOW_WIDTH;
    int image_height = Config::WINDOW_HEIGHT;

    Camera& cam = m_scene->m_camera;

    auto sphere1 = std::make_shared<Sphere::Sphere>(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f);
    auto sphere2 = std::make_shared<Sphere::Sphere>(glm::vec3(2.1f, 2.1f, 2.1f), 0.5f);
    m_scene->addObject(sphere1);
    m_scene->addObject(sphere2);

    // ray tracing loops
    //#pragma omp parallel for schedule(dynamic, 1)
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            glm::vec3 pixelColor(0.0, 0.0, 0.0);
            
            for (int s = 0; s < Config::SAMPLES; ++s) {
                // Normalize x, y coordinates in the scene
                float u = (i + Random::randomDouble()) / (image_width - 1);
                float v = (j + Random::randomDouble()) / (image_height - 1);
                Ray::Ray ray = cam.getRay(u, v);

                float t;

                if (sphere1->intersect(ray, t)) {
                    pixelColor += glm::vec3(255.0, 0.0, 0.0);
                } else if(sphere2->intersect(ray, t)) {
                    pixelColor += glm::vec3(0.0, 0.0, 255.0);
                } else {
                    pixelColor += glm::vec3(0.0, 0.0, 0.0);
                }
            }

            pixelColor /= static_cast<float>(Config::SAMPLES);

            // Apply gamma correction (optional)
            pixelColor = glm::sqrt(pixelColor / 255.0f);


            int ir = static_cast<int>(glm::clamp(pixelColor.r * 255.0f, 0.0f, 255.0f));
            int ig = static_cast<int>(glm::clamp(pixelColor.g * 255.0f, 0.0f, 255.0f));
            int ib = static_cast<int>(glm::clamp(pixelColor.b * 255.0f, 0.0f, 255.0f));

            sdl_ctx->setPixel(i, j, ir, ig, ib);
        }
    }

    // update screen
    sdl_ctx->updateScreen();

    SDL_Event event;
    while(isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { 
                std::cout << "SDL_QUIT event detected. Exiting...\n";
                isRunning = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) { 
                    std::cout << "ESCAPE key pressed. Exiting...\n";
                    isRunning = false;
                }
            }
        }

        //do stuff

        //SDL_Delay(10);
    }
}