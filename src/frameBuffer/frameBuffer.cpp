#include <fstream>
#include <cassert>

#include "frameBuffer.h"
#include "globals/globals.h"

FrameBuffer::FrameBuffer(int width, int height) 
    : m_width(width), m_height(height), m_pixels(width * height) {}


void FrameBuffer::setPixel(int x, int y, const glm::vec3& color) {
    assert(x >= 0 && x < m_width);
    assert(y >= 0 && y < m_height);
    m_pixels[y * m_width + x] = color;
}

void FrameBuffer::accumulatePixel(int x, int y, const glm::vec3& color) {
    assert(x >= 0 && x < m_width);
    assert(y >= 0 && y < m_height);
    m_pixels[y * m_width + x] += color;
}

glm::vec3 FrameBuffer::getPixel(int x, int y) const {
    assert(x >= 0 && x < m_width);
    assert(y >= 0 && y < m_height);
    return m_pixels[y * m_width + x];
}

void FrameBuffer::saveAsPPM(const std::string& filename) const {
    std::ofstream file(filename);
    file << "P3\n" << m_width << " " << m_height << "\n255\n";

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            glm::vec3 color = getPixel(x, y);

            // Normalize color with number of samples
            color /= static_cast<float>(Config::SAMPLES);

            // gamma correction
            color = glm::sqrt(color);

            //from 0 to 1 to 0 to 255
            int r = static_cast<int>(255.999 * glm::clamp(color.r, 0.0f, 1.0f));
            int g = static_cast<int>(255.999 * glm::clamp(color.g, 0.0f, 1.0f));
            int b = static_cast<int>(255.999 * glm::clamp(color.b, 0.0f, 1.0f));
            
            file << r << " " << g << " " << b << "\n";
        }
    }

    file.close();
}