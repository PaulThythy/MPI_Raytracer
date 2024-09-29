#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hitableObject.h"
#include "ray.h"
#include "material.h"

namespace Hitable {

    struct Triangle : public Hitable::HitableObject {
        glm::vec3 m_v0, m_v1, m_v2;
        std::shared_ptr<Material> m_material;

        inline Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const std::shared_ptr<Material>& mat)
            : m_v0(v0), m_v1(v1), m_v2(v2), m_material(mat) {}

        inline ~Triangle() {}

        inline bool intersect(const Ray::Ray& ray, float& t_min, float& t_max, HitRecord& rec) const override {
            //TODO remake intersection test
            return true;
        }

        inline glm::vec3 getNormal(const glm::vec3& point) const override {
            return glm::normalize(glm::cross(m_v1 - m_v0, m_v2 - m_v0));
        }
    };
}

#endif