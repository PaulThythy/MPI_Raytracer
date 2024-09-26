#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <iostream>

#include "camera.h"
#include "../math/hitableObject.h"


struct Scene {

    Camera m_camera;
    std::vector<std::shared_ptr<Hitable::HitableObject>> m_objects;

    inline Scene(const Camera& _camera) : m_camera(_camera) {}

    inline void addObject(const std::shared_ptr<Hitable::HitableObject>& object) {
        m_objects.push_back(object);
    }
};

#endif