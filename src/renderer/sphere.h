#ifndef SPHERE_H
#define SPHERE_H

#include "hitableObject.h"
#include "ray.h"
#include "material.h"

namespace Hitable {

    struct Sphere : public Hitable::HitableObject {
        glm::vec3 m_center;
        float m_radius;
        std::shared_ptr<Material> m_material;

        inline Sphere(const glm::vec3& center, const float radius, const std::shared_ptr<Material>& mat): m_center(center), m_radius(radius), m_material(mat) {}

        inline ~Sphere() {}

        virtual bool intersect(const Ray::Ray& ray, float& t_min, float& t_max, HitRecord& rec) const override {
            glm::vec3 oc = ray.m_origin - m_center;
            float a = glm::dot(ray.m_direction, ray.m_direction);
            float half_b = glm::dot(oc, ray.m_direction);
            float c = glm::dot(oc, oc) - m_radius * m_radius;
            float discriminant = half_b * half_b - a * c;

            if (discriminant > 0) {
                float root = glm::sqrt(discriminant);
                float temp = (-half_b - root) / a;
                if (temp < t_max && temp > t_min) {
                    rec.t = temp;
                    rec.m_point = ray.at(rec.t);
                    glm::vec3 outward_normal = (rec.m_point - m_center) / m_radius;
                    rec.setFaceNormal(ray, outward_normal);
                    rec.m_material = m_material;
                    return true;
                }
                temp = (-half_b + root) / a;
                if (temp < t_max && temp > t_min) {
                    rec.t = temp;
                    rec.m_point = ray.at(rec.t);
                    glm::vec3 outward_normal = (rec.m_point - m_center) / m_radius;
                    rec.setFaceNormal(ray, outward_normal);
                    rec.m_material = m_material;
                    return true;
                }
            }
            return false;
        }

        inline glm::vec3 getNormal(const glm::vec3& point) const override {
            return glm::normalize(point - m_center);
        }
    };
}

#endif