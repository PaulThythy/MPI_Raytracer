#ifndef HITABLEOBJECT_H
#define HITABLEOBJECT_H

#include <glm/glm.hpp>

namespace Hitable {
    
    struct HitableObject {
        virtual ~HitableObject() = default;
        virtual bool intersect(const Hitable::Ray& ray, float& t) const = 0;
		//virtual glm::vec3 getNormal(const glm::vec3& point) const = 0;
    };
}

#endif