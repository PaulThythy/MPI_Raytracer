#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <iostream>

#include "camera.h"
#include "renderer/hitableObject.h"
#include "renderer/sphere.h"
#include "renderer/ray.h"
#include "renderer/triangle.h"
#include "renderer/light.h"
#include "globals/globals.h"
#include "frameBuffer/frameBuffer.h"
#include "random/random.h"
#include "context/SDL_context.h"


struct Scene {

    Camera m_camera;
    std::vector<std::shared_ptr<Hitable::HitableObject>> m_objects;
    std::vector<std::shared_ptr<Light>> m_lights;

    inline Scene(const Camera& camera) : m_camera(camera) { init(); }

    inline void addObject(const std::shared_ptr<Hitable::HitableObject>& object) { m_objects.push_back(object); }

    inline void addLight(const std::shared_ptr<Light>& light) { m_lights.push_back(light); }

    inline void init() {
        m_objects = {
            std::make_shared<Hitable::Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.f, glm::vec3(255.0f, 0.0f, 0.0f)),
            std::make_shared<Hitable::Sphere>(glm::vec3(2.f, 2.f, 2.f), 0.5f, glm::vec3(0.0f, 0.0f, 255.0f)),
            std::make_shared<Hitable::Sphere>(glm::vec3(0.f, 2.f, -4.f), 1.0f, glm::vec3(0.0f, 255.0f, 0.0f)),
            std::make_shared<Hitable::Triangle>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(255.0f, 255.0f, 255.0f))
        };
    }

    inline void traceRays(SDL::SDL_context* sdlCtx) {
        int image_width = Config::WINDOW_WIDTH;
        int image_height = Config::WINDOW_HEIGHT;

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
                    Hitable::Ray ray = m_camera.getRay(u, v);

                    float t;
                    bool hit = false;
                    glm::vec3 hitColor(0.0f, 0.0f, 0.0f);

                    for (const auto& object : m_objects) {
                        if (object->intersect(ray, t)) {

                            hitColor = object->m_albedo;
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

                sdlCtx->setPixel(i, j, ir, ig, ib);
                fb.setPixel(i, j, color);

            }
        }

        // update screen
        sdlCtx->updateScreen();
        fb.saveAsPPM("image.ppm");
    }
};

#endif