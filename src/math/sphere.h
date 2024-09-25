#ifndef SPHERE_H
#define SPHERE_H

#include "hitableObject.h"
#include "ray.h"

namespace Sphere {

    struct Sphere : public Hitable::HitableObject {
        glm::vec3 m_center;
        double m_radius;
        //add material member

        inline Sphere(const glm::vec3& _center, const double _radius): m_center(_center), m_radius(_radius) {}

        inline ~Sphere() {}

        inline bool intersect(const Ray::Ray& ray, float& t) const override {
            glm::vec3 oc = ray.m_origin - m_center;
            double a = glm::dot(ray.m_direction, ray.m_direction);
            double b = 2.0f * glm::dot(oc, ray.m_direction);
            double c = glm::dot(oc, oc) - m_radius * m_radius;
            double discriminant = b * b - 4 * a * c;

            if (discriminant < 0) {
                return false;
            }
            else {
                t = (-b - sqrt(discriminant)) / (2.0f * a);
                return true;
            }
        }
    };
}

#endif