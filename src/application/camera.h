#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "../math/ray.h"
#include "../globals/globals.h"
#include "../random/random.h"

namespace Camera {
    struct Camera {
        
        glm::vec3 m_origin;

        // Auxiliary vectors
        glm::vec3 m_right;
        glm::vec3 m_lowerLeftCorner;

        // Informations sur le viewport
        glm::vec3 m_viewportWidthVec;
        glm::vec3 m_viewportHeightVec;

        glm::vec3 m_lookFrom;
        glm::vec3 m_lookAt;
        glm::vec3 m_up;
        double m_vfov;         // Vertical field of view in degrees
        double m_aspectRatio;  // Aspect ratio (width/height)

        double m_lensRadius;   // Lens radius for depth of field

        // camera basis vectors
        glm::vec3 m_u, m_v, m_w;


        Camera(const glm::vec3& lookFrom, const glm::vec3& lookAt, const glm::vec3& up,
            double vfov = 90.0, double aspectRatio = Config::WINDOW_WIDTH/Config::WINDOW_HEIGHT, double aperture = 0., double focusDist = 1.) 
        {
            m_lensRadius = aperture / 2;
            m_lookFrom = lookFrom;
            m_lookAt = lookAt;
            m_up = up;
            m_vfov = vfov;
            m_aspectRatio = aspectRatio;

            double theta = glm::radians(vfov);
            double h = tan(theta / 2);
            double viewportHeight = 2.0 * h;
            double viewportWidth = aspectRatio * viewportHeight;

            m_w = glm::normalize(lookFrom - lookAt);
            m_u = glm::normalize(glm::cross(up, m_w));
            m_v = glm::cross(m_w, m_u);

            m_origin = lookFrom;
            m_viewportWidthVec = multiplyVectWithScalar(multiplyVectWithScalar(m_u, viewportWidth), focusDist);
            m_viewportHeightVec = multiplyVectWithScalar(multiplyVectWithScalar(m_v, viewportHeight), focusDist);

            m_lowerLeftCorner = m_origin - m_viewportWidthVec / 2.0f - m_viewportHeightVec / 2.0f - multiplyVectWithScalar(m_w, focusDist);
        }

        // Fonction pour obtenir un rayon en fonction des coordonnées u et v
        inline Ray::Ray getRay(double u, double v) {
            glm::vec3 rd = multiplyVectWithScalar(Random::randomInUnitDisk(), m_lensRadius);
            glm::vec3 offset = m_u * rd.x + m_v * rd.y;

            glm::vec3 direction = m_lowerLeftCorner + multiplyVectWithScalar(m_viewportWidthVec, u) + multiplyVectWithScalar(m_viewportHeightVec, v) - m_origin - offset;

            return Ray::Ray(m_origin + offset, direction);
        }

        inline glm::vec3 multiplyVectWithScalar(const glm::vec3& vec, double scalar) const {
            return glm::vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
        }

    };
}

#endif