#ifndef MATERIAL_H
#define MATERIAL_H

#ifdef _WIN32
	#include <glm.hpp>
    #include <gtc/constants.hpp>
#elif __linux__
	#include <glm/glm.hpp>
    #include <glm/gtc/constants.hpp>
#endif

#include "renderer/ray.h"
#include "renderer/hitRecord.h"
#include "random/random.h"

namespace PBR {

    struct Material {
        glm::vec3 m_albedo;                             //base color
        float m_metallic;                               //metallic (1.0) or dielectric (0.0)
        float m_roughness;                              //affects surface microstructure, affecting the sharpness of specular reflections
        float m_ambientOcclusion;                       //factor that indirectly reduces ambient light in hidden areas
        float m_reflectance;                            //reflection factor for dielectrics


        Material(
            const glm::vec3& albedo = glm::vec3(1.0),
            float metallic = 0.0f,
            float roughness = 0.5,
            float ambientOcclusion = 1.0f,
            float reflectance = 0.04f                   //default value for dielectrics
        ) : m_albedo(albedo),
            m_metallic(metallic),
            m_roughness(roughness),
            m_ambientOcclusion(ambientOcclusion),
            m_reflectance(reflectance) {}
        
        virtual ~Material() = default;

        virtual inline bool scatter(const Ray::Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray::Ray& scattered) {
            //view vector
            glm::vec3 V = -glm::normalize(rayIn.m_direction);

            //normal vector of the surface
            glm::vec3 N = rec.m_normal;

            //generate a random vector in the hemisphere of the hit point for diffuse reflection
            //TODO test with randomInHemisphere function
            glm::vec3 L = glm::normalize(N + Random::randomInUnitSphere());

            glm::vec3 H = glm::normalize(V + L);

            //coefficients
            float NdotL = std::max(glm::dot(N, L), 0.0f);
            float NdotV = std::max(glm::dot(N, V), 0.0f);
            //float NdotH = std::max(glm::dot(N, H), 0.0f);
            float VdotH = std::max(glm::dot(V, H), 0.0f);

            float D = distributionGGX(N, H, m_roughness);
            float G = geometrySmith(N, V, L, m_roughness);
            glm::vec3 F0 = glm::mix(glm::vec3(m_reflectance), m_albedo, m_metallic);
            glm::vec3 F = fresnelSchlick(VdotH, F0);

            // BRDF
            glm::vec3 nominator = D * G * F;
            float denominator = 4.0f * NdotV * NdotL + 0.0001f;
            glm::vec3 specular = nominator / denominator;

            //Lambertian (diffuse component)
            glm::vec3 kD = (1.0f - F) * (1.0f - m_metallic);
            glm::vec3 diffuse = (kD * m_albedo) / glm::pi<float>();

            //reflected energy
            glm::vec3 radiance = (diffuse + specular) * NdotL;

            //attenuation
            attenuation = radiance * m_ambientOcclusion;

            //scattered ray
            scattered = Ray::Ray(rec.m_point, L);

            return true;
        } 

        virtual glm::vec3 emitted() const {
            return glm::vec3(0.0f);
        }

        inline float distributionGGX(const glm::vec3& N, const glm::vec3& H, float roughness) {
            float a = roughness * roughness;
            float a2 = a * a;
            float NdotH = std::max(glm::dot(N, H), 0.0f);
            float NdotH2 = NdotH * NdotH;

            float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
            denom = glm::pi<float>() * denom * denom;

            return a2 / denom;
        }

        inline float geometrySchlickGGX(float NdotV, float roughness) {
            float r = (roughness + 1.0f);
            float k = (r * r) / 8.0f;

            float denom = NdotV * (1.0f - k) + k;
            return NdotV / denom;
        }

        inline float geometrySmith(const glm::vec3& N, const glm::vec3& V, const glm::vec3& L, float roughness) {
            float NdotV = std::max(glm::dot(N, V), 0.0f);
            float NdotL = std::max(glm::dot(N, L), 0.0f);
            float ggx2 = geometrySchlickGGX(NdotV, roughness);
            float ggx1 = geometrySchlickGGX(NdotL, roughness);

            return ggx1 * ggx2;
        }

        inline glm::vec3 fresnelSchlick(float cosTheta, const glm::vec3& F0) {
            return F0 + (1.0f - F0) * glm::pow(1.0f - cosTheta, 5.0f);
        }           
    };
}

#endif