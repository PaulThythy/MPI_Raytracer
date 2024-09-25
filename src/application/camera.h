#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "../math/ray.h"

namespace Camera {
    
    struct Camera {
        glm::vec3 m_origin;

        //helpers
        glm::vec3 m_right;
        glm::vec3 m_lowerLeftCorner;

        //viewport info
        glm::vec3 m_viewportWidthVec;
        glm::vec3 m_viewportHeightVec;

        glm::vec3 m_orig;

        glm::vec3 m_lookFrom;
        glm::vec3 m_lookAt;
        glm::vec3 m_up;
        double m_vfov;
        double m_aspectRatio;

        double m_lensRadius;

        Camera::Camera( const glm::vec3& _origin, const glm::vec3& _lookAt, const glm::vec3& _up, 
                        double ang = 40, double aspect = 1080./1920., double aperture = 0, double focusDist = 1.) 
                        : m_origin(_origin), m_lookAt(_lookAt), m_up(_up), m_lensRadius(aperture/2.) {

            const double theta = glm::radians(m_vfov);
            const double viewportWidth = 2.0 * tan(theta / 2);
            const double viewportHeight = m_aspectRatio * viewportWidth;

            const glm::vec3 lookVec = glm::normalize(_origin - _lookAt);
            const glm::vec3 u = glm::normalize(glm::cross(_up, lookVec));
            const glm::vec3 v = glm::cross(lookVec, u);

            m_up = glm::normalize(v);
            m_right = glm::normalize(u);

            m_lowerLeftCorner = _origin - viewportWidth / 2. * focusDist * m_right - viewportHeight / 2. * focusDist * m_up - focusDist * lookVec;
        
            m_viewportWidthVec = viewportWidth * focusDist * m_right;
            m_viewportHeightVec = viewportHeight * focusDist * _up;

            m_orig = m_lowerLeftCorner - m_lookFrom;
        }
        
        
        inline Ray& getRay(double x, double y) const {
            if (0. == lensRadius) 
                return Ray(m_lookFrom, m_orig + x * m_viewportWidthVec + y * m_viewportHeightVec);

            const glm::vec3 rvec = m_lensRadius * /*getRandomInUnitSphere()*/;
            const glm::vec3 off = rvec.X * m_right + rvec.Y * m_up;

            return Ray(m_lookFrom + off, m_orig + x * m_viewportWidthVec + y * m_viewportHeightVec - off);
        }

        inline 
    }

}

#endif