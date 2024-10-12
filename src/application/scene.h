#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <iostream>

#include "camera.h"
#include "renderer/hitableObject.h"
#include "renderer/ray.h"
#include "renderer/light.h"
#include "renderer/hitRecord.h"
#include "renderer/hitableList.h"
#include "globals/globals.h"
#include "frameBuffer/frameBuffer.h"
#include "random/random.h"
#include "context/SDL_context.h"
#include "context/MPI_context.h"


struct Scene {

    Camera m_camera;
    HitableList m_world;
    std::vector<std::shared_ptr<Lighting::Light>> m_lights;

    inline Scene() : m_camera(
        glm::vec3(10.0f, 2.0f, 3.0f),                                                                                   //lookFrom
        glm::vec3(0.0f, 0.0f, 0.0f),                                                                                    //lookAt
        glm::vec3(0.0f, -1.0f, 0.0f),                                                                                   //up
        90.0f,                                                                                                          //fov                    
        static_cast<float>(Config::WINDOW_WIDTH) / static_cast<float>(Config::WINDOW_HEIGHT),                           //aspectRatio
        0.0f,                                                                                                           //aperture                 
        1.0f                                                                                                            //focusDist
        ) { init(); }

    void init();

    glm::vec3 rayColor(const Ray::Ray& ray, HitableList& world, const std::vector<std::shared_ptr<Lighting::Light>>& lights, int bounces);

    void render(MPI::MPI_context* mpiCtx, SDL::SDL_context* sdlCtx);
};

#endif