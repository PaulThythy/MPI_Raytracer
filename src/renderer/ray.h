#ifndef RAY_H
#define RAY_H

#ifdef _WIN32
	#include <glm.hpp>
#elif __linux__
	#include <glm/glm.hpp>
#endif

namespace Ray {
    struct Ray {
        glm::vec3 m_origin;
        glm::vec3 m_direction;

        Ray() {}

        inline Ray(const glm::vec3& _origin, const glm::vec3& _direction): m_origin(_origin) {
            m_direction = glm::normalize(_direction);
        }

        inline ~Ray() {}

        inline glm::vec3 at(float t) const {
            glm::vec3 point;
            point.x = this->m_origin.x + this->m_direction.x * t;
            point.y = this->m_origin.y + this->m_direction.y * t;
            point.z = this->m_origin.z + this->m_direction.z * t;
            return point;
        }
    };
}

#endif