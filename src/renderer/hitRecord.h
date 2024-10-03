#ifndef HITRECORD_H
#define HITRECORD_H

#ifdef _WIN32
	#include <glm.hpp>
#elif __linux__
	#include <glm/glm.hpp>
#endif
#include <memory>

#include "ray.h"

namespace PBR {
    struct Material;
}

struct HitRecord {
    glm::vec3 m_point;
    glm::vec3 m_normal;
    float t;
    bool m_isFrontFace;
    std::shared_ptr<PBR::Material> m_material;
    //isEmissive

    inline void setFaceNormal(const Ray::Ray& ray, const glm::vec3& outwardNormal) {
        m_isFrontFace = glm::dot(ray.m_direction, outwardNormal) < 0;
        m_normal = m_isFrontFace ? outwardNormal : -outwardNormal;
    }
};

#endif