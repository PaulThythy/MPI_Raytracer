#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hitableObject.h"
#include "ray.h"

namespace Hitable {

    struct Triangle : public Hitable::HitableObject {
        glm::vec3 m_v0, m_v1, m_v2;
        //add material member

        inline Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& albedo)
            : m_v0(v0), m_v1(v1), m_v2(v2) {
            
            m_albedo = albedo; 
        }

        inline ~Triangle() {}

        inline bool intersect(const Hitable::Ray& ray, float& t) const override {
            const float EPSILON = 1e-8f;
            glm::vec3 edge1 = m_v1 - m_v0;
            glm::vec3 edge2 = m_v2 - m_v0;

            glm::vec3 h = glm::cross(ray.m_direction, edge2);
            float a = glm::dot(edge1, h);

            if (fabs(a) < EPSILON)
                return false; // The ray is parallel to the triangle

            float f = 1.0f / a;
            glm::vec3 s = ray.m_origin - m_v0;
            float u = f * glm::dot(s, h);

            if (u < 0.0f || u > 1.0f)
                return false;

            glm::vec3 q = glm::cross(s, edge1);
            float v = f * glm::dot(ray.m_direction, q);

            if (v < 0.0f || u + v > 1.0f)
                return false;

            // Calculating t to find the intersection on the radius
            float temp_t = f * glm::dot(edge2, q);

            if (temp_t > EPSILON) { // intersection found
                t = temp_t;
                return true;
            } else { // The intersection is behind the ray
                return false;
            }
        }

        inline glm::vec3 getNormal(const glm::vec3& point) const override {
            return glm::normalize(glm::cross(m_v1 - m_v0, m_v2 - m_v0));
        }
    };
}

#endif