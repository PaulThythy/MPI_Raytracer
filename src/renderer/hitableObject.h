#ifndef HITABLEOBJECT_H
#define HITABLEOBJECT_H

#include <glm/glm.hpp>

#include "ray.h"
#include "hitRecord.h"

namespace Hitable {
    
    struct HitableObject {
        virtual ~HitableObject() = default;
        virtual bool intersect(const Ray::Ray& ray, float& t_min, float& t_max, HitRecord& rec) const = 0;
		virtual glm::vec3 getNormal(const glm::vec3& point) const = 0;
        //TODO bounding boxes
    };
}

#endif