#include "scene.h"

void Scene::init() {
    auto matSphere1 = std::make_shared<PBR::Material>(glm::vec3(1.0f, 0.0f, 0.0f));
    auto matSphere2 = std::make_shared<PBR::Material>(glm::vec3(0.0f, 0.0f, 1.0f));
    auto matSphere3 = std::make_shared<PBR::Material>(glm::vec3(0.0f, 1.0f, 0.0f));
    auto matSphere4 = std::make_shared<PBR::Material>(glm::vec3(1.0f, 1.0f, 0.0f));
    auto matSphere5 = std::make_shared<PBR::Material>(glm::vec3(0.0f, 1.0f, 1.0f));
    auto matSphere6 = std::make_shared<PBR::Material>(glm::vec3(1.0f, 0.0f, 1.0f));

    m_world.m_objects = {
        std::make_shared<Hitable::Sphere>(glm::vec3(0.0f, -50.0f, 0.0f), 50.f, matSphere1),
        std::make_shared<Hitable::Sphere>(glm::vec3(-2.5f, 0.0f, 7.5f), 0.5f, matSphere2),
        std::make_shared<Hitable::Sphere>(glm::vec3(-2.0f, 0.7f, 5.2f), 1.0f, matSphere3),
        std::make_shared<Hitable::Sphere>(glm::vec3(-1.85f, -0.07f, 6.1f), 0.2f, matSphere4),
        std::make_shared<Hitable::Sphere>(glm::vec3(4.4f, 0.65f, 0.0f), 0.8f, matSphere5),
        std::make_shared<Hitable::Sphere>(glm::vec3(2.8f, -0.35f, 7.4f), 0.1f, matSphere6)
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

void Scene::render(MPI::MPI_context* mpiCtx, SDL::SDL_context* sdlCtx) {
    int rank = mpiCtx->getRank();
    int image_width = Config::WINDOW_WIDTH;
    int image_height = Config::WINDOW_HEIGHT;

    int num_workers = Config::NB_WORKERS;

    int samples_per_worker = Config::SAMPLES / num_workers;
    int remainder_samples = Config::SAMPLES % num_workers;

    int num_samples = samples_per_worker + (rank < remainder_samples ? 1 : 0);

    int total_samples = 0;

    std::vector<float> local_sum_buffer(image_width * image_height * 3, 0.0f);
    std::vector<float> global_sum_buffer;
    if (rank == 0) {
        global_sum_buffer.resize(local_sum_buffer.size(), 0.0f);
    }

    for(int s = 0 ; s < num_samples ; s++) {
        std::vector<float> local_sample_buffer(image_width * image_height * 3, 0.0f);

        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                float u = (i + Random::randomFloat(0.0f, 1.0f)) / (image_width - 1);
                float v = (j + Random::randomFloat(0.0f, 1.0f)) / (image_height - 1);
                Ray::Ray ray = m_camera.getRay(u, v);

                glm::vec3 sampleColor = rayColor(ray, m_world, m_lights, Config::BOUNCES);

                int idx = (j * image_width + i) * 3;
                local_sample_buffer[idx]     += sampleColor.r;
                local_sample_buffer[idx + 1] += sampleColor.g;
                local_sample_buffer[idx + 2] += sampleColor.b;
            }
        }

        std::vector<float> global_sample_buffer;
        if(rank == 0) {
            global_sample_buffer.resize(local_sample_buffer.size(), 0.0f);
        }
        MPI_Reduce(local_sample_buffer.data(), global_sample_buffer.data(), local_sample_buffer.size(), MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

        if(rank == 0) {
            for(size_t idx = 0; idx < global_sample_buffer.size(); ++idx) {
                global_sum_buffer[idx] += global_sample_buffer[idx];
            }

            total_samples += num_workers;

            for (int j = 0; j < image_height; ++j) {
                for (int i = 0; i < image_width; ++i) {
                    int idx = (j * image_width + i) * 3;
                    float r = global_sum_buffer[idx] / total_samples;
                    float g = global_sum_buffer[idx + 1] / total_samples;
                    float b = global_sum_buffer[idx + 2] / total_samples;

                    int ir = static_cast<int>(glm::clamp(r * 255.99f, 0.0f, 255.99f));
                    int ig = static_cast<int>(glm::clamp(g * 255.99f, 0.0f, 255.99f));
                    int ib = static_cast<int>(glm::clamp(b * 255.99f, 0.0f, 255.99f));

                    sdlCtx->setPixel(i, j, ir, ig, ib);
                }
            }

            sdlCtx->updateScreen();
        }
    }
}