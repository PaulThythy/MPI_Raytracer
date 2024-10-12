#ifndef RANDOM_H
#define RANDOM_H

#ifdef _WIN32
	#include <glm.hpp>
#elif __linux__
	#include <glm/glm.hpp>
#endif

#include <random>

#include "globals/globals.h"

namespace Random {
    inline double randomFloat(float min, float max) {
        static std::uniform_real_distribution<float> distribution(min, max);
        static std::mt19937 generator(std::random_device{}());
        return distribution(generator);
    }


    // Function for generating a random point in a drive (for depth of field)
    inline glm::vec3 randomInUnitDisk() {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(distribution(generator), distribution(generator), 0) - glm::vec3(1, 1, 0);
        } while (glm::dot(p, p) >= 1.0);
        return p;
    }

    inline glm::vec3 randomInUnitSphere() {
        while (true) {
            glm::vec3 p = glm::vec3(
                randomFloat(-1.0f, 1.0f),
                randomFloat(-1.0f, 1.0f),
                randomFloat(-1.0f, 1.0f)
            );
            if (glm::length(p) * glm::length(p) >= 1.0f) continue;
            return p;
        }
    }

    inline glm::vec3 randomUnitVector() {
        float a = randomFloat(0.0f, 2.0f * Config::PI);
        float z = randomFloat(-1.0f, 1.0f);
        float r = sqrt(1.0f - z * z);
        return glm::vec3(r * glm::cos(a), r * glm::sin(a), z);
    }

    inline glm::vec3 randomInHemisphere(const glm::vec3& normal) {
        glm::vec3 in_unit_sphere = randomInUnitSphere();
        if (glm::dot(in_unit_sphere, normal) > 0.0f)
            return in_unit_sphere;
        else
            return -in_unit_sphere;
    }
}

#endif