#include MATERIAL_H
#define MATERIAL_H

//TODO to implement

struct Material {
    virtual bool scatter(const Ray::Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray::Ray& scattered) const = 0;
}


struct Lambertian : public Material {
    glm::vec3 m_albedo;

    Lambertian(const glm::vec3& albedo) : m_albedo(albedo) {}

    virtual bool scatter(const Ray::Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation, Ray::Ray& scattered) const override {
        glm::vec3 scatterDirection = rec.normal + randomUnitVector();
        scattered = Ray::Ray(rec.point, scatterDirection);
        attenuation = albedo;
        return true;
    }
}

#endif