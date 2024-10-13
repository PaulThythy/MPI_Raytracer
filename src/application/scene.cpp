#include "scene.h"

void Scene::init() {
    auto matSphere1 = std::make_shared<PBR::Material>(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.5f, 1.0f);
    auto matSphere2 = std::make_shared<PBR::Material>(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.1f, 1.0f);
    auto matSphere3 = std::make_shared<PBR::Material>(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.8f, 1.0f);

    m_world.m_objects = {
        std::make_shared<Hitable::Sphere>(glm::vec3(0.0f, 2.f, 0.0f), 2.f, matSphere1),
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

void Scene::render(MPI::MPI_context* mpiCtx, SDL::SDL_context* sdlCtx) {
    /*int rank = mpiCtx->getRank();
    int num_processes = mpiCtx->getNumTasks();
    int num_workers = Config::NB_WORKERS; // Le processus de rang 0 est le maître

    int samples_per_worker = Config::SAMPLES / num_workers;
    int remainder_samples = Config::SAMPLES % num_workers;

    int assigned_samples = 0;
    if (rank != 0) {
        assigned_samples = samples_per_worker + ((rank <= remainder_samples) ? 1 : 0);
    }

    int max_assigned_samples = samples_per_worker + ((remainder_samples > 0) ? 1 : 0);

    int image_width = Config::WINDOW_WIDTH;
    int image_height = Config::WINDOW_HEIGHT;

    std::vector<float> local_buffer(image_width * image_height * 3, 0.0f);
    std::vector<float> global_buffer;
    if (rank == 0) {
        global_buffer.resize(local_buffer.size(), 0.0f);
    }

    int total_samples_so_far = 0;

    for (int s = 1; s <= max_assigned_samples; ++s) {
        // Réinitialiser local_buffer
        std::fill(local_buffer.begin(), local_buffer.end(), 0.0f);

        // Déterminer si le processus contribue cette itération
        bool worker_has_sample = false;
        if (rank != 0 && s <= assigned_samples) {
            worker_has_sample = true;

            // Calcul des échantillons
            for (int j = 0; j < image_height; ++j) {
                for (int i = 0; i < image_width; ++i) {
                    float u = (i + Random::randomFloat(0.0f, 1.0f)) / (image_width - 1);
                    float v = (j + Random::randomFloat(0.0f, 1.0f)) / (image_height - 1);
                    Ray::Ray ray = m_camera.getRay(u, v);

                    glm::vec3 sampleColor = rayColor(ray, m_world, m_lights, Config::BOUNCES);

                    int idx = (j * image_width + i) * 3;

                    local_buffer[idx] += sampleColor.r;
                    local_buffer[idx + 1] += sampleColor.g;
                    local_buffer[idx + 2] += sampleColor.b;
                }
            }
        }

        // Tous les processus appellent MPI_Reduce
        if (rank == 0) {
            MPI_Reduce(MPI_IN_PLACE, local_buffer.data(), local_buffer.size(), MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
        }
        else {
            MPI_Reduce(local_buffer.data(), nullptr, local_buffer.size(), MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
        }

        if (rank == 0) {
            // Accumuler les nouvelles données dans global_buffer
            for (size_t idx = 0; idx < global_buffer.size(); ++idx) {
                global_buffer[idx] += local_buffer[idx];
            }

            // Calculer le nombre d'échantillons contribué cette itération
            int samples_contributed_this_iteration = 0;
            if (s <= samples_per_worker) {
                samples_contributed_this_iteration = num_workers;
            }
            else if (s == samples_per_worker + 1 && remainder_samples > 0) {
                samples_contributed_this_iteration = remainder_samples;
            }
            else {
                samples_contributed_this_iteration = 0;
            }

            total_samples_so_far += samples_contributed_this_iteration;

            if (samples_contributed_this_iteration > 0) {
                for (int j = 0; j < image_height; ++j) {
                    for (int i = 0; i < image_width; ++i) {
                        int idx = (j * image_width + i) * 3;
                        float r = global_buffer[idx] / total_samples_so_far;
                        float g = global_buffer[idx + 1] / total_samples_so_far;
                        float b = global_buffer[idx + 2] / total_samples_so_far;

                        int ir = static_cast<int>(glm::clamp(r * 255.0f, 0.0f, 255.0f));
                        int ig = static_cast<int>(glm::clamp(g * 255.0f, 0.0f, 255.0f));
                        int ib = static_cast<int>(glm::clamp(b * 255.0f, 0.0f, 255.0f));

                        sdlCtx->setPixel(i, j, ir, ig, ib);
                    }
                }

                sdlCtx->updateScreen();
            }
        }
    }

    if (rank == 0) {
        std::cout << "Rendering finished" << std::endl;
    }*/

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
        std::cout << "render finished" << std::endl;
    }

    fb.saveAsPPM("image.ppm");
}
