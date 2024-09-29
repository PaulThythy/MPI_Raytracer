#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

#include "renderer/ray.h"
#include "renderer/hitRecord.h"
#include "random/random.h"

struct Material {
    virtual bool scatter(const Ray::Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray::Ray& scattered) const = 0;

    virtual glm::vec3 getAlbedo() const { return glm::vec3(0.0f); }
};


struct Lambertian : public Material {
    glm::vec3 m_albedo;

    Lambertian(const glm::vec3& albedo) : m_albedo(albedo) {}
    ~Lambertian() {}

    inline bool scatter(const Ray::Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray::Ray& scattered) const override {
        glm::vec3 scatter_direction = Random::randomInHemisphere(rec.m_normal);
        scattered = Ray::Ray(rec.m_point, scatter_direction);
        attenuation = m_albedo;
        return true;
    }

    inline glm::vec3 getAlbedo() const override {
        return m_albedo;
    }
};

struct Metal : public Material {
    glm::vec3 m_albedo;
    float m_fuzz;

    Metal(const glm::vec3& a, float f) : m_albedo(a), m_fuzz(f < 1 ? f : 1) {}
    ~Metal() {}

    inline bool scatter(const Ray::Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray::Ray& scattered) const override {
        glm::vec3 reflected = glm::reflect(glm::normalize(rayIn.m_direction), rec.m_normal);
        scattered = Ray::Ray(rec.m_point, reflected + m_fuzz * Random::randomInUnitSphere());
        attenuation = m_albedo;
        return (glm::dot(scattered.m_direction, rec.m_normal) > 0);
    }

    inline glm::vec3 getAlbedo() const override {
        return m_albedo;
    }
};

struct Dielectric : public Material {
    float m_refIdx;

    Dielectric(float ri) : m_refIdx(ri) {}
    ~Dielectric() {}

    inline bool scatter(const Ray::Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray::Ray& scattered) const override {
        attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
        float etai_over_etat = rec.m_isFrontFace ? (1.0f / m_refIdx) : m_refIdx;

        glm::vec3 unit_direction = glm::normalize(rayIn.m_direction);
        float cos_theta = fmin(glm::dot(-unit_direction, rec.m_normal), 1.0f);
        float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0f) {
            glm::vec3 reflected = glm::reflect(unit_direction, rec.m_normal);
            scattered = Ray::Ray(rec.m_point, reflected);
            return true;
        }

        float reflect_prob = schlick(cos_theta, etai_over_etat);
        if (Random::randomFloat(0.0f, 1.0f) < reflect_prob) {
            glm::vec3 reflected = glm::reflect(unit_direction, rec.m_normal);
            scattered = Ray::Ray(rec.m_point, reflected);
        } else {
            glm::vec3 refracted = glm::refract(unit_direction, rec.m_normal, etai_over_etat);
            scattered = Ray::Ray(rec.m_point, refracted);
        }

        return true;
    }

    inline float schlick(float cosine, float ref_idx) const {
        float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
    }
};

struct Emissive : public Material {
    glm::vec3 m_emitColor;

    Emissive(const glm::vec3& color) : m_emitColor(color) {}

    inline bool scatter(const Ray::Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray::Ray& scattered) const override {
        return false;
    }

    glm::vec3 emitted() const {
        return m_emitColor;
    }

    inline glm::vec3 getAlbedo() const override {
        return glm::vec3(0.0f);
    }
};

#endif