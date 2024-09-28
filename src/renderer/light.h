#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

//TODO to implement

struct Light {
    virtual glm::vec3 getLightDirection(const glm::vec3& point) const = 0;
    virtual glm::vec3 getIntensity(const glm::vec3& point) const = 0;
    virtual ~Light() = default;
};


struct PointLight : public Light {
    glm::vec3 m_position;
    glm::vec3 m_intensity;

    PointLight(const glm::vec3& pos, const glm::vec3& inten)
        : m_position(pos), m_intensity(inten) {}

    virtual glm::vec3 getLightDirection(const glm::vec3& point) const override {
        return glm::normalize(m_position - point);
    }

    virtual glm::vec3 getIntensity(const glm::vec3& point) const override {
        float distanceSquared = glm::distance(m_position, point);
        return m_intensity / (distanceSquared); 
    }
};


struct DirectionalLight : public Light {
    glm::vec3 m_direction;
    glm::vec3 m_intensity;

    DirectionalLight(const glm::vec3& dir, const glm::vec3& inten)
        : m_direction(glm::normalize(dir)), m_intensity(inten) {}

    virtual glm::vec3 getLightDirection(const glm::vec3& point) const override {
        return glm::normalize(-m_direction); 
    }

    virtual glm::vec3 getIntensity(const glm::vec3& point) const override {
        return m_intensity; 
    }
};

//TODO other types of lights : Area, Spot, Background

#endif // LIGHT_H
