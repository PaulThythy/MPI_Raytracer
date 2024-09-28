#include <iostream>

#include <memory>

#include "application.h"
#include "renderer/sphere.h"
#include "renderer/triangle.h"

Application::Application(int argc, char *argv[]) {
    m_isRunning = true;

    m_mpiCtx = new MPI::MPI_context(argc, argv);

    glm::vec3 lookFrom(13.0f, 2.0f, 3.0f); 
    glm::vec3 lookAt(0.0f, 0.0f, 0.0f);    
    glm::vec3 up(0.0f, -1.0f, 0.0f);       
    float vfov = 90.0f;                    
    float aspectRatio = static_cast<float>(Config::WINDOW_WIDTH) / static_cast<float>(Config::WINDOW_HEIGHT);
    float aperture = 0.0f;                  
    float focusDist = 1.0f;

    Camera cam(lookFrom, lookAt, up, vfov, aspectRatio, aperture, focusDist);
    m_scene = std::make_unique<Scene>(cam);

    if(m_mpiCtx->getRank() == 0) {
        m_sdlCtx = new SDL::SDL_context();

        if (m_sdlCtx->initSDL()) {
            execute();
        } else {
            m_isRunning = false;
        }
    }
}

Application::~Application() {
    if (m_mpiCtx != NULL) {
        m_mpiCtx->endMPI();
        delete m_mpiCtx;
    }
    
    if (m_sdlCtx != NULL) {
        m_sdlCtx->endSDL();
        delete m_sdlCtx;
    }
}

/*glm::vec3 rayColor(const Ray::Ray& ray, const Hitable::HitableObject& world, int depth) {
    Hitable::HitRecord rec;
    float t;
    if (depth <= 0)
        return glm::vec3(0.0f, 0.0f, 0.0f);

    if (world.intersect(ray, t, rec)) {
        Ray::Ray scattered;
        glm::vec3 attenuation;
        if (rec.materialPtr->scatter(ray, rec, attenuation, scattered))
            return attenuation * rayColor(scattered, world, depth - 1);
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // gradient background
    glm::vec3 unitDirection = glm::normalize(ray.direction);
    float t = 0.5f * (unitDirection.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}*/

void Application::execute() {
    m_scene->render(m_sdlCtx);

    SDL_Event event;
    while(m_isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { 
                std::cout << "SDL_QUIT event detected. Exiting...\n";
                m_isRunning = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) { 
                    std::cout << "ESCAPE key pressed. Exiting...\n";
                    m_isRunning = false;
                }
            }

            //TODO add events (for moving camera for example)
        }

        //do stuff

        //SDL_Delay(10);
    }
}