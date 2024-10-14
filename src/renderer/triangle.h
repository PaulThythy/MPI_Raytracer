#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hitableObject.h"
#include "ray.h"
#include "material.h"

namespace Hitable {

    struct Triangle : public Hitable::HitableObject {
        glm::vec3 m_v0, m_v1, m_v2;
        std::shared_ptr<PBR::Material> m_material;

        inline Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const std::shared_ptr<PBR::Material>& mat)
            : m_v0(v0), m_v1(v1), m_v2(v2), m_material(mat) {}

        inline ~Triangle() {}

        inline bool intersect(const Ray::Ray& ray, float& t_min, float& t_max, HitRecord& rec) const override {
            const float EPSILON = 1e-8f;
            glm::vec3 edge1 = m_v1 - m_v0;
            glm::vec3 edge2 = m_v2 - m_v0;

            glm::vec3 h = glm::cross(ray.m_direction, edge2);
            float a = glm::dot(edge1, h);

            if (fabs(a) < EPSILON)
                return false; // Le rayon est parallèle au triangle

            float f = 1.0f / a;
            glm::vec3 s = ray.m_origin - m_v0;
            float u = f * glm::dot(s, h);

            if (u < 0.0f || u > 1.0f)
                return false; // Le point est en dehors du triangle

            glm::vec3 q = glm::cross(s, edge1);
            float v = f * glm::dot(ray.m_direction, q);

            if (v < 0.0f || u + v > 1.0f)
                return false; // Le point est en dehors du triangle

            float t = f * glm::dot(edge2, q);

            if (t >= t_min && t <= t_max) {
                rec.m_t = t;
                rec.m_p = ray.at(t);
                rec.m_normal = getNormal(rec.m_p);
                rec.m_material = m_material;
                return true;
            } else {
                return false; // L'intersection est en dehors des limites [t_min, t_max]
            }
        }

        inline glm::vec3 getNormal(const glm::vec3& point) const override {
            return glm::normalize(glm::cross(m_v1 - m_v0, m_v2 - m_v0));
        }
    };
}

#endif