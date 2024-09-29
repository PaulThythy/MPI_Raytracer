#include "scene.h"

void Scene::init() {
    //TODO add a namespace for the materials
    auto matSphere1 = std::make_shared<Lambertian>(glm::vec3(1.0f, 0.0f, 0.0f)); 
    auto matSphere2 = std::make_shared<Lambertian>(glm::vec3(0.0f, 0.0f, 1.0f)); 
    auto matSphere3 = std::make_shared<Lambertian>(glm::vec3(0.0f, 1.0f, 0.0f));

    m_world.m_objects = {
        std::make_shared<Hitable::Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.f, matSphere1),
        std::make_shared<Hitable::Sphere>(glm::vec3(2.f, 2.f, 2.f), 0.5f, matSphere2),
        std::make_shared<Hitable::Sphere>(glm::vec3(0.f, 2.f, -4.f), 1.0f, matSphere3)
    };

    m_lights = {
        Lighting::DirectionalLight(glm::vec3(0.f, 0.f, -6.f), glm::vec3(255, 255, 255), 500.0, glm::vec3(0.0, 0.0, 0.0))
    };
}

glm::vec3 Scene::rayColor(const Ray::Ray& ray, HitableList& world, const std::vector<Light>& lights, int bounces) {
    HitRecord rec;

    if (bounces <= 0)
        return glm::vec3(0.0f);
        
    float t_min = 0.001f;
    float t_max = FLT_MAX;

    if (world.intersect(ray, t_min, t_max, rec)) {
        glm::vec3 color(0.0f);

        // check if material is emissive
        Emissive* emissive = dynamic_cast<Emissive*>(rec.m_material.get());
        if (emissive) {
            color += emissive->emitted();
        }

        // compute direct lighting
        for (const auto& light : lights) {
            glm::vec3 to_light = glm::normalize(light.m_position - rec.m_point);
            Ray::Ray shadow_ray(rec.m_point, to_light);
            HitRecord shadow_rec;

            // check if the point is in the shadow
            float length = glm::length(light.m_position - rec.m_point);
            bool in_shadow = world.intersect(shadow_ray, t_min, length, shadow_rec);
            if (!in_shadow) {
                float ndotl = glm::max(glm::dot(rec.m_normal, to_light), 0.0f);
                color += rec.m_material->getAlbedo() * light.m_color * light.m_intensity * ndotl;
            }
        }

        // Reflection/Refraction
        Ray::Ray scattered;
        glm::vec3 attenuation;
        if (rec.m_material->scatter(ray, rec, attenuation, scattered)) {
            color += attenuation * rayColor(scattered, world, lights, bounces - 1);
        }

        return color;
    }

    // background color
    glm::vec3 unit_direction = glm::normalize(ray.m_direction);
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}


void Scene::render(SDL::SDL_context* sdlCtx) {
    int image_width = Config::WINDOW_WIDTH;
    int image_height = Config::WINDOW_HEIGHT;

    FrameBuffer fb(image_width, image_height);

    for (int s = 0; s < Config::SAMPLES; ++s) {
        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                float u = (i + Random::randomFloat(0.0f, 1.0f)) / (image_width - 1);
                float v = (j + Random::randomFloat(0.0f, 1.0f)) / (image_height - 1);
                Ray::Ray ray = m_camera.getRay(u, v);

                glm::vec3 sampleColor = rayColor(ray, m_world, m_lights, Config::BOUNCES);

                fb.accumulatePixel(i, j, sampleColor);

                glm::vec3 avgColor = fb.getPixel(i, j) / static_cast<float>(s + 1);

                int ir = static_cast<int>(glm::clamp(avgColor.r * 255.0f, 0.0f, 255.0f));
                int ig = static_cast<int>(glm::clamp(avgColor.g * 255.0f, 0.0f, 255.0f));
                int ib = static_cast<int>(glm::clamp(avgColor.b * 255.0f, 0.0f, 255.0f));

                sdlCtx->setPixel(i, j, ir, ig, ib);
            }
        }

        sdlCtx->updateScreen();
    }

    fb.saveAsPPM("image.ppm");
}