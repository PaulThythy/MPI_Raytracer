#ifndef HITRECORD_H
#define HITRECORD_H

#include <glm/glm.hpp>
#include <memory>

#include "renderer/ray.h"
#include "renderer/material.h"

//TODO to implement

struct HitRecord {
    glm::vec3 m_point;
    glm::vec3 m_normal;
    float t;
    bool m_isFrontFace;
    std::shared_ptr<Material> m_material;

    inline void setFaceNormal(const Ray::Ray& ray, const glm::vec3& outwardNormal) {
        m_isFrontFace = glm::dot(ray.m_direction, outwardNormal) < 0;
        m_normal = m_isFrontFace ? outwardNormal : -outwardNormal;
    }
};

#endif