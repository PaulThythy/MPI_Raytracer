#ifndef LIGHT_H
#define LIGHT_H

#ifdef _WIN32
	#include <glm.hpp>
#elif __linux__
	#include <glm/glm.hpp>
#endif

namespace Lighting {
    enum struct LightType : char {
		DIRECTIONAL
	};

    struct Light {
        glm::vec3 m_position;
        glm::vec3 m_color;     
        float m_intensity;     
        LightType m_type;

        Light(const glm::vec3& position, const glm::vec3& color, float intensity, LightType type)
            : m_position(position), m_color(color), m_intensity(intensity), m_type(type) {}

        virtual ~Light() = default;

        virtual glm::vec3 getDirection(const glm::vec3& point, float& distance) const = 0;

        virtual float getAttenuation(float distance) const = 0;

        virtual glm::vec3 getEmission() const {
            return glm::vec3(0.0f); //By default, no emission
        }
    };

    struct DirectionalLight : public Light {
        glm::vec3 m_direction; // must be normalized

        DirectionalLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float intensity)
            : Light(position, color, intensity, LightType::DIRECTIONAL), m_direction(glm::normalize(direction)) {}

        inline glm::vec3 getDirection(const glm::vec3& point, float& distance) const override {
            distance = FLT_MAX;
            return m_direction;
        }

        inline float getAttenuation(float distance) const override {
            return 1.0f; 
        }
    };
}

//TODO other types of lights : Point, Area, Spot, Background

#endif // LIGHT_H
