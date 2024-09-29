#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glm/glm.hpp>
#include <vector>


class FrameBuffer {

public:
    FrameBuffer(int width, int height);

    void setPixel(int x, int y, const glm::vec3& color);
    void accumulatePixel(int x, int y, const glm::vec3& color);

    glm::vec3 getPixel(int x, int y) const;

    void saveAsPPM(const std::string& filename) const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    int m_width, m_height;
    std::vector<glm::vec3> m_pixels;
};

#endif