#include "App.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace App {

void Application::start() {
  TGAImage framebuffer(width_, height_, TGAImage::RGB);
  std::srand(std::time({}));

  loadModel(modelPath_, framebuffer);

  framebuffer.write_tga_file(outputFile_);
}

void Application::addTriangle(const Models::Points3D &points,
                              const glm::vec3 &normal, TGAImage &framebuffer,
                              TGAColor color) {
  // back-face culling: a triangle only faces the viewer (looking down -z)
  // when its normal has a positive z component
  if (normal.z <= 0.0f)
    return;

  renderer::Renderer::draw(renderer::Primitive::Triangle, points, framebuffer,
                           color);
}

void Application::loadModel(const std::string &filename,
                            TGAImage &framebuffer) {
  model::Model mdl(filename);
  std::cout << "Number of vertices: " << mdl.nverts() << std::endl;
  std::cout << "Number of faces: " << mdl.nfaces() << std::endl;

  for (int i = 0; i < mdl.nfaces(); i++) {
    std::vector<int> face = mdl.face(i);
    glm::vec3 v0 = mdl.vert(face[0]);
    glm::vec3 v1 = mdl.vert(face[1]);
    glm::vec3 v2 = mdl.vert(face[2]);
    glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);

    int x0 = static_cast<int>((v0.x + 1.0f) * framebuffer.width() / 2.0f);
    int y0 = static_cast<int>((v0.y + 1.0f) * framebuffer.height() / 2.0f);
    int x1 = static_cast<int>((v1.x + 1.0f) * framebuffer.width() / 2.0f);
    int y1 = static_cast<int>((v1.y + 1.0f) * framebuffer.height() / 2.0f);
    int x2 = static_cast<int>((v2.x + 1.0f) * framebuffer.width() / 2.0f);
    int y2 = static_cast<int>((v2.y + 1.0f) * framebuffer.height() / 2.0f);

    TGAColor faceColor = {static_cast<uint8_t>(std::rand() % 255),
                          static_cast<uint8_t>(std::rand() % 255),
                          static_cast<uint8_t>(std::rand() % 255), 255};

    addTriangle(Models::Points3D{x0, y0, x1, y1, x2, y2}, normal, framebuffer,
                faceColor);
  }
}

} // namespace App
