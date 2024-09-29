#ifndef HITRECORD_H
#define HITRECORD_H

#include <glm/glm.hpp>
#include <memory>

#include "ray.h"

struct Material;

struct HitRecord {
    glm::vec3 m_point;
    glm::vec3 m_normal;
    float t;
    bool m_isFrontFace;
    std::shared_ptr<Material> m_material;
    //isEmissive

    inline void setFaceNormal(const Ray::Ray& ray, const glm::vec3& outwardNormal) {
        m_isFrontFace = glm::dot(ray.m_direction, outwardNormal) < 0;
        m_normal = m_isFrontFace ? outwardNormal : -outwardNormal;
    }
};

#endif