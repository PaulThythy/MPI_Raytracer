#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

namespace Hitable {
    struct Ray {
        glm::vec3 m_origin;
        glm::vec3 m_direction;

        inline Ray(const glm::vec3& _origin, const glm::vec3& _direction): m_origin(_origin) {
            m_direction = glm::normalize(_direction);
        }

        inline ~Ray() {}

        inline glm::vec3 getPoint(double t) {
            glm::vec3 point;
            point.x = this->m_origin.x + this->m_direction.x * t;
            point.y = this->m_origin.y + this->m_direction.y * t;
            point.z = this->m_origin.z + this->m_direction.z * t;
            return point;
        }
    };
}

#endif