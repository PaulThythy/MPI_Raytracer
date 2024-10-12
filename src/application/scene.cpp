#include "scene.h"

void Scene::init() {
    //TODO add a namespace for the materials
    auto matSphere1 = std::make_shared<PBR::Material>(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.5f, 1.0f); 
    auto matSphere2 = std::make_shared<PBR::Material>(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.1f, 1.0f); 
    auto matSphere3 = std::make_shared<PBR::Material>(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.8f, 1.0f);

    m_world.m_objects = {
        std::make_shared<Hitable::Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.f, matSphere1),
        std::make_shared<Hitable::Sphere>(glm::vec3(2.f, 2.f, 2.f), 0.5f, matSphere2),
        std::make_shared<Hitable::Sphere>(glm::vec3(0.f, 2.f, -4.f), 1.0f, matSphere3)
    };

    m_lights = {
        std::make_shared<Lighting::DirectionalLight>(glm::vec3(0.f, -3.f, 5.f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(255, 255, 255), 500.0f)/*,
        Lighting::DirectionalLight(glm::vec3(0.f, 5.f, 0.f), glm::vec3(255, 255, 255), 100.0, glm::vec3(0.0, 0.0, 0.0))*/
    };
}

glm::vec3 Scene::rayColor(const Ray::Ray& ray, HitableList& world, const std::vector<std::shared_ptr<Lighting::Light>>& lights, int bounces) {
    HitRecord rec;

    if (bounces <= 0)
        return glm::vec3(0.0f);

    float t_min = 0.001f;
    float t_max = FLT_MAX;

    if (world.intersect(ray, t_min, t_max, rec)) {
        Ray::Ray scattered;
        glm::vec3 attenuation;

        if (rec.m_material->scatter(ray, rec, attenuation, scattered)) {
            return attenuation * rayColor(scattered, world, lights, bounces - 1);
        } else {
            return rec.m_material->emitted();
        }
    }

    // returns black if no intersection
    //return glm::vec3(0.0f);

    // or returns a gradient from white to blue
    glm::vec3 unit_direction = glm::normalize(ray.m_direction);
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

void Scene::render(SDL::SDL_context* sdlCtx) {
    /*int image_width = Config::WINDOW_WIDTH;
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

    fb.saveAsPPM("image.ppm");*/

    int num_processes = mpiCtx->getNumTasks();
    int rank = mpiCtx->getRank();

    int samples_per_process = Config::SAMPLES / num_processes;
    int remainder_samples = Config::SAMPLES % num_processes;

    int start_sample = rank * samples_per_process + std::min(rank, remainder_samples);
    int end_sample = start_sample + samples_per_process + (rank < remainder_samples ? 1 : 0);

    std::cout << "rank : " << rank << ", start_sample : " << start_sample << ", end_sample : " << end_sample << std::endl;
}