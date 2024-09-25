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

        Camera( const glm::vec3& _origin, const glm::vec3& _lookAt, const glm::vec3& _up, 
                        double ang = 40, double aspect = 1080./1920., double aperture = 0, double focusDist = 1.) 
                        : m_origin(_origin), m_lookAt(_lookAt), m_up(_up), m_lensRadius(aperture/2.) {

            double theta = glm::radians(m_vfov);
            double viewportWidth = 2.0 * tan(theta / 2);
            double viewportHeight = m_aspectRatio * viewportWidth;

            glm::vec3 lookVec = glm::normalize(_origin - _lookAt);
            glm::vec3 u = glm::normalize(glm::cross(_up, lookVec));
            glm::vec3 v = glm::cross(lookVec, u);

            m_up = glm::normalize(v);
            m_right = glm::normalize(u);

            //m_lowerLeftCorner = _origin - viewportWidth / 2. * focusDist * m_right - viewportHeight / 2. * focusDist * m_up - focusDist * lookVec;

            double half_viewport_width = (viewportWidth / 2.0) * focusDist;
            double half_viewport_height = (viewportHeight / 2.0) * focusDist;
            glm::vec3 half_width = multiplyVectWithScalar(m_right, half_viewport_width);
            glm::vec3 half_height = multiplyVectWithScalar(m_up, half_viewport_height);
            glm::vec3 focal_length = multiplyVectWithScalar(lookVec, focusDist);
            m_lowerLeftCorner = m_origin - half_width - half_height - focal_length;


            m_viewportWidthVec = multiplyVectWithScalar(m_right, viewportWidth * focusDist);
            m_viewportHeightVec = multiplyVectWithScalar(m_up, viewportHeight * focusDist);

            m_orig = m_lowerLeftCorner - m_lookFrom;
        }
        
        
        /*inline Ray& getRay(double x, double y) const {
            if (0. == lensRadius) 
                return Ray(m_lookFrom, m_orig + x * m_viewportWidthVec + y * m_viewportHeightVec);

            const glm::vec3 rvec = m_lensRadius * getRandomInUnitSphere();
            const glm::vec3 off = rvec.X * m_right + rvec.Y * m_up;

            return Ray(m_lookFrom + off, m_orig + x * m_viewportWidthVec + y * m_viewportHeightVec - off);
        }*/

        inline Ray::Ray getRay(double x, double y) const {
            glm::vec3 direction = m_lowerLeftCorner + multiplyVectWithScalar(m_viewportWidthVec, x) + multiplyVectWithScalar(m_viewportHeightVec, y) - m_origin;
            return Ray::Ray(m_origin, glm::normalize(direction));
        }

        inline glm::vec3 multiplyVectWithScalar(const glm::vec3& vec, double scalar) {
            return glm::vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
        }

    };
}

#endif