
#include "Model/model.h"
#include "Models.h"
#include "Renderer/LineRenderer.h"
#include "Renderer/TriangleRenderer.h"
#include "tgaimage.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void readObjFile(const std::string fileName, TGAImage &framebuffer) {

  // load the model from the .obj file
  model::Model model(fileName);
  std::cout << "Number of vertices: " << model.nverts() << std::endl;
  std::cout << "Number of faces: " << model.nfaces() << std::endl;
  // draw lines between the vertices of each face
  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<int> face = model.face(i);
    for (int j = 0; j < 3; j++) {
      glm::vec3 v0 = model.vert(face[j]);
      glm::vec3 v1 = model.vert(face[(j + 1) % 3]);
      std::cout << "number of faces: " << model.nfaces() << std::endl;
      std::cout << "face[" << i << "]: " << face[j] << ", " << face[(j + 1) % 3]
                << std::endl;

      int x0 = static_cast<int>((v0.x + 1.0f) * framebuffer.width() / 2.0f);
      int y0 = static_cast<int>((v0.y + 1.0f) * framebuffer.height() / 2.0f);
      int x1 = static_cast<int>((v1.x + 1.0f) * framebuffer.width() / 2.0f);
      int y1 = static_cast<int>((v1.y + 1.0f) * framebuffer.height() / 2.0f);
      renderer::LineRenderer::drawOptimizedLine(x0, y0, x1, y1, framebuffer,
                                                red);
    }
  }
}

void triangleObj(const std::string fileName, TGAImage &framebuffer) {

  // load the model from the .obj file
  model::Model model(fileName);
  std::cout << "Number of vertices: " << model.nverts() << std::endl;
  std::cout << "Number of faces: " << model.nfaces() << std::endl;
  // fill each face's triangle
  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<int> face = model.face(i);
    glm::vec3 v0 = model.vert(face[0]);
    glm::vec3 v1 = model.vert(face[1]);
    glm::vec3 v2 = model.vert(face[2]);

    int x0 = static_cast<int>((v0.x + 1.0f) * framebuffer.width() / 2.0f);
    int y0 = static_cast<int>((v0.y + 1.0f) * framebuffer.height() / 2.0f);
    int x1 = static_cast<int>((v1.x + 1.0f) * framebuffer.width() / 2.0f);
    int y1 = static_cast<int>((v1.y + 1.0f) * framebuffer.height() / 2.0f);
    int x2 = static_cast<int>((v2.x + 1.0f) * framebuffer.width() / 2.0f);
    int y2 = static_cast<int>((v2.y + 1.0f) * framebuffer.height() / 2.0f);

    TGAColor faceColor = {static_cast<uint8_t>(std::rand() % 255),
                          static_cast<uint8_t>(std::rand() % 255),
                          static_cast<uint8_t>(std::rand() % 255), 255};

    // renderer::TriangleRenderer::fillTriangle(x0, y0, x1, y1, x2, y2,
    //                                          framebuffer, faceColor);
    auto point = Models::Points3D{x0, y0, x1, y1, x2, y2};
    renderer::TriangleRenderer::fillTriangle(point, framebuffer, faceColor);
  }
}

void drawTriangle(int ax, int ay, int bx, int by, int cx, int cy,
                  TGAImage &framebuffer, TGAColor color) {
  // sort the vertices, a,b,c in ascending y order (bubblesort yay!)
  if (ay > by) {
    std::swap(ax, bx);
    std::swap(ay, by);
  }
  if (ay > cy) {
    std::swap(ax, cx);
    std::swap(ay, cy);
  }
  if (by > cy) {
    std::swap(bx, cx);
    std::swap(by, cy);
  }
  renderer::LineRenderer::drawOptimizedLine(ax, ay, bx, by, framebuffer, green);
  renderer::LineRenderer::drawOptimizedLine(bx, by, cx, cy, framebuffer, green);
  renderer::LineRenderer::drawOptimizedLine(cx, cy, ax, ay, framebuffer, red);
}

int main(int argc, char **argv) {
  constexpr int width = 800;
  constexpr int height = 800;
  TGAImage framebuffer(width, height, TGAImage::RGB);
  std::srand(std::time({}));

  triangleObj("obj/diablo3_pose/diablo3_pose.obj", framebuffer);
  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
