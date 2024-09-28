#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <iostream>

#include "camera.h"
#include "renderer/hitableObject.h"
#include "renderer/sphere.h"
#include "renderer/triangle.h"


struct Scene {

    Camera m_camera;
    std::vector<std::shared_ptr<Hitable::HitableObject>> m_objects;

    inline Scene(const Camera& _camera) : m_camera(_camera) {}

    inline void addObject(const std::shared_ptr<Hitable::HitableObject>& object) {
        m_objects.push_back(object);
    }

    inline void init() {
        m_objects = {
            std::make_shared<Hitable::Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.f),
            std::make_shared<Hitable::Sphere>(glm::vec3(2.f, 2.f, 2.f), 0.5f),
            std::make_shared<Hitable::Sphere>(glm::vec3(0.f, 2.f, -4.f), 1.0f),
            std::make_shared<Hitable::Triangle>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f))
        };
    }

    inline void trace() {

    }
};

#endif