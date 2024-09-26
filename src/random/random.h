#ifndef RANDOM_H
#define RANDOM_H

#include <glm/glm.hpp>
#include <random>

namespace Random {
    inline double randomDouble() {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
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
}

#endif