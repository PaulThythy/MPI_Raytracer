#ifndef CAMERA_H
#define CAMERA_H

#ifdef _WIN32
	#include <glm.hpp>
    #include <gtc/constants.hpp>
#elif __linux__
	#include <glm/glm.hpp>
    #include <glm/gtc/constants.hpp>
#endif

#include "renderer/ray.h"
#include "globals/globals.h"
#include "random/random.h"


struct Camera {
    // Auxiliary vectors
    glm::vec3 m_right;
    glm::vec3 m_lowerLeftCorner;

    // viewport infos
    glm::vec3 m_viewportWidthVec;
    glm::vec3 m_viewportHeightVec;

    glm::vec3 m_lookFrom;
    glm::vec3 m_lookAt;
    glm::vec3 m_up;
    float m_vfov;           // Vertical field of view in degrees
    float m_aspectRatio;    // Aspect ratio (width/height)
    float m_aperture;       // Aperture for depth of field
    float m_lensRadius;     // Lens radius for depth of field
    float m_focusDist;      // Focus distance

    // camera basis vectors
    glm::vec3 m_u, m_v, m_w;


    Camera(const glm::vec3& lookFrom, const glm::vec3& lookAt, const glm::vec3& up,
        float vfov = 90.0f, float aspectRatio = float(Config::WINDOW_WIDTH)/float(Config::WINDOW_HEIGHT), float aperture = 0., float focusDist = 1.) 
    {
        m_lensRadius = aperture / 2;
        m_lookFrom = lookFrom;
        m_lookAt = lookAt;
        m_up = up;
        m_vfov = vfov;
        m_aspectRatio = aspectRatio;
        m_aperture = aperture;
        m_focusDist = focusDist;

        float theta = glm::radians(vfov);
        float h = tan(theta / 2);
        float viewportHeight = 2.0 * h;
        float viewportWidth = aspectRatio * viewportHeight;

        m_w = glm::normalize(lookFrom - lookAt);
        m_u = glm::normalize(glm::cross(up, m_w));
        m_v = glm::cross(m_w, m_u);

        m_viewportWidthVec = m_u *  viewportWidth * focusDist;
        m_viewportHeightVec = m_v *  viewportHeight * focusDist;

        m_lowerLeftCorner = m_lookFrom - m_viewportWidthVec / 2.0f - m_viewportHeightVec / 2.0f - m_w * focusDist;
    }

    /*inline void updateCamera() {
        m_lensRadius = m_aperture / 2.0f;

        float theta = glm::radians(m_vfov);
        float h = tan(theta / 2);
        float viewportHeight = 2.0f * h;
        float viewportWidth = m_aspectRatio * viewportHeight;

        m_w = glm::normalize(m_lookFrom - m_lookAt);
        m_u = glm::normalize(glm::cross(m_up, m_w));
        m_v = glm::cross(m_w, m_u);

        m_viewportWidthVec = m_u * viewportWidth * m_focusDist;
        m_viewportHeightVec = m_v * viewportHeight * m_focusDist;

        m_lowerLeftCorner = m_lookFrom - m_viewportWidthVec / 2.0f - m_viewportHeightVec / 2.0f - m_w * m_focusDist;
    }*/

    inline Ray::Ray getRay(float u, float v) {
        glm::vec3 rd = Random::randomInUnitDisk() * m_lensRadius;
        glm::vec3 offset = m_u * rd.x + m_v * rd.y;

        glm::vec3 direction = m_lowerLeftCorner + m_viewportWidthVec * u + m_viewportHeightVec * v - m_lookFrom - offset;

        return Ray::Ray(m_lookFrom + offset, direction);
    }

};

#endif