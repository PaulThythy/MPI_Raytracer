#ifndef HITABLELIST_H
#define HITABLELIST_H

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "renderer/sphere.h"
#include "renderer/material.h"
#include "renderer/hitRecord.h"
#include "renderer/ray.h"
#include "renderer/hitableObject.h"

struct HitableList {
    std::vector<std::shared_ptr<Hitable::HitableObject>> m_objects;

    inline HitableList() { init(); }

    inline void add(std::shared_ptr<Hitable::HitableObject> object) { m_objects.push_back(object); }

    inline void init() {
        auto matSphere1 = std::make_shared<Lambertian>(glm::vec3(1.0f, 0.0f, 0.0f)); 
        auto matSphere2 = std::make_shared<Lambertian>(glm::vec3(0.0f, 0.0f, 1.0f)); 
        auto matSphere3 = std::make_shared<Lambertian>(glm::vec3(0.0f, 1.0f, 0.0f));

        m_objects = {
            std::make_shared<Hitable::Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.f, matSphere1),
            std::make_shared<Hitable::Sphere>(glm::vec3(2.f, 2.f, 2.f), 0.5f, matSphere2),
            std::make_shared<Hitable::Sphere>(glm::vec3(0.f, 2.f, -4.f), 1.0f, matSphere3)
        };
    }

    inline bool intersect(const Ray::Ray& ray, float& t_min, float& t_max, HitRecord& rec) {
        HitRecord temp_rec;
        bool hit_anything = false;
        float closest_so_far = t_max;

        for (const auto& object : m_objects) {
            if (object->intersect(ray, t_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

};

#endif