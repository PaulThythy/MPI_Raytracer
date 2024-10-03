#ifndef HITABLELIST_H
#define HITABLELIST_H

#ifdef _WIN32
	#include <glm.hpp>
#elif __linux__
	#include <glm/glm.hpp>
#endif
#include <vector>
#include <memory>

#include "renderer/sphere.h"
#include "renderer/material.h"
#include "renderer/hitRecord.h"
#include "renderer/ray.h"
#include "renderer/hitableObject.h"

struct HitableList {
    std::vector<std::shared_ptr<Hitable::HitableObject>> m_objects;

    HitableList() = default;

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