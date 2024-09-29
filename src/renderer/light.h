#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct Light {
    glm::vec3 m_position;
    glm::vec3 m_color;
    float m_intensity;
    
    virtual ~Light() = default;
};

struct DirectionalLight : public Light {
    glm::vec3 m_direction; 

    DirectionalLight(const glm::vec3& pos, const glm::vec3& color, const float intens, const glm::vec3& dir)
        : m_direction(dir) {
            
        m_position = pos; m_color = color; m_intensity = intens;
    }
};


/*struct PointLight : public Light {
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
};*/

//TODO other types of lights : Point, Area, Spot, Background

#endif // LIGHT_H
