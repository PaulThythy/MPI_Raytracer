#include <iostream>

#include <memory>

#include "application.h"
#include "renderer/sphere.h"
#include "renderer/triangle.h"
#include "globals/globals.h"
#include "random/random.h"
#include "frameBuffer/frameBuffer.h"

Application::Application(int argc, char *argv[]) {
    isRunning = true;

    mpi_ctx = new MPI::MPI_context(argc, argv);

    glm::vec3 lookFrom(13.0f, 2.0f, 3.0f); 
    glm::vec3 lookAt(0.0f, 0.0f, 0.0f);    
    glm::vec3 up(0.0f, -1.0f, 0.0f);       
    float vfov = 90.0f;                    
    float aspectRatio = static_cast<float>(Config::WINDOW_WIDTH) / static_cast<float>(Config::WINDOW_HEIGHT);
    float aperture = 0.0f;                  
    float focusDist = 1.0f;

    Camera cam(lookFrom, lookAt, up, vfov, aspectRatio, aperture, focusDist);
    m_scene = std::make_unique<Scene>(cam);

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
    int image_width = Config::WINDOW_WIDTH;
    int image_height = Config::WINDOW_HEIGHT;

    Camera& cam = m_scene->m_camera;

    FrameBuffer fb(image_width, image_height);

    // ray tracing loops
    //#pragma omp parallel for schedule(dynamic, 1)
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            glm::vec3 pixelColor(0.0, 0.0, 0.0);
        
            for (int s = 0; s < Config::SAMPLES; ++s) {
                // Normalize x, y coordinates in the scene
                float u = (i + Random::randomDouble()) / (image_width - 1);
                float v = (j + Random::randomDouble()) / (image_height - 1);
                Hitable::Ray ray = cam.getRay(u, v);

                float t;
                bool hit = false;
                glm::vec3 hitColor(0.0f, 0.0f, 0.0f);

                for (const auto& object : m_scene->m_objects) {
                    if (object->intersect(ray, t)) {

                        if (object == m_scene->m_objects[0]) { 
                            hitColor = glm::vec3(255.0f, 0.0f, 0.0f);
                        }
                        else if (object == m_scene->m_objects[1]) { 
                            hitColor = glm::vec3(0.0f, 0.0f, 255.0f);
                        }
                        else if (object == m_scene->m_objects[2]) { 
                            hitColor = glm::vec3(0.0f, 255.0f, 0.0f);
                        }
                        else if (object == m_scene->m_objects[3]) { 
                            hitColor = glm::vec3(255.0f, 255.0f, 255.0f); 
                        }
                        hit = true;

                        break; // stop after first intersection
                    }
                }

                if (hit) {
                    pixelColor += hitColor;
                } else {
                    pixelColor += glm::vec3(0.0f, 0.0f, 0.0f);
                }
            }

            pixelColor /= static_cast<float>(Config::SAMPLES);

            // Apply gamma correction (optional)
            pixelColor = glm::sqrt(pixelColor / 255.0f);


            int ir = static_cast<int>(glm::clamp(pixelColor.r * 255.0f, 0.0f, 255.0f));
            int ig = static_cast<int>(glm::clamp(pixelColor.g * 255.0f, 0.0f, 255.0f));
            int ib = static_cast<int>(glm::clamp(pixelColor.b * 255.0f, 0.0f, 255.0f));
            glm::vec3 color(ir, ig, ib);

            sdl_ctx->setPixel(i, j, ir, ig, ib);
            fb.setPixel(i, j, color);

        }
    }

    // update screen
    sdl_ctx->updateScreen();
    fb.saveAsPPM("image.ppm");

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