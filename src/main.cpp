
#include "Model/model.h"
#include "tgaimage.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void setThickPixel(int x, int y, TGAImage &frameBuffer, TGAColor color,
                   int thickness) {
  int half = thickness / 2;
  for (int dy = -half; dy <= half; dy++) {
    for (int dx = -half; dx <= half; dx++) {
      frameBuffer.set(x + dx, y + dy, color);
    }
  }
}

void drawOptimizedLine(int ax, int ay, int bx, int by, TGAImage &framebuffer,
                       TGAColor color, int thickness = 0) {

  bool steep = std::abs(ax - bx) < std::abs(ay - by);
  if (steep) { // if the line is steep, we transpose the image
    std::swap(ax, ay);
    std::swap(bx, by);
  }
  if (ax > bx) { // make it left−to−right
    std::swap(ax, bx);
    std::swap(ay, by);
  }
  int y = ay;
  int ierror = 0;
  for (int x = ax; x <= bx; x++) {
    if (steep) // if transposed, de−transpose
      framebuffer.set(y, x, color);
    else
      framebuffer.set(x, y, color);
    ierror += 2 * std::abs(by - ay);
    y += (by > ay ? 1 : -1) * (ierror > bx - ax);
    ierror -= 2 * (bx - ax) * (ierror > bx - ax);
  }
}
void drawLine(int ax, int ay, int bx, int by, TGAImage &frameBuffer,
              TGAColor color) {
  bool steep = std::abs(ax - bx) < std::abs(ay - by);
  if (steep) {
    // first vertically transpose the line
    std::swap(ax, ay);
    std::swap(bx, by);
  }
  if (ax > bx) { // make it left−to−right
    std::swap(ax, bx);
    std::swap(ay, by);
  }
  for (float x = ax; x <= bx; x += 1.0) {
    // Calculate the interpolation factor t based on the current x position
    float t = (x - ax) / static_cast<float>(bx - ax);
    float y = std::round(ay + (by - ay) * t);
    if (steep)
      // if transposed, we need to swap x and y when setting the pixel
      frameBuffer.set(y, x, color);
    else
      frameBuffer.set(x, y, color);
  }
}

void readObjFile(const std::string fileName, TGAImage &framebuffer) {

  // load the model from the .obj file
  Model model(fileName);
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
      drawOptimizedLine(x0, y0, x1, y1, framebuffer, red);
    }
  }
}

int main(int argc, char **argv) {
  constexpr int width = 800;
  constexpr int height = 800;
  TGAImage framebuffer(width, height, TGAImage::RGB);

  // int ax = 7, ay = 3;
  // int bx = 12, by = 37;
  // int cx = 62, cy = 53;
  // drawLine(ax, ay, bx, by, framebuffer, blue);
  // drawLine(cx, cy, bx, by, framebuffer, green);
  // drawLine(cx, cy, ax, ay, framebuffer, yellow);
  // drawLine(ax, ay, cx, cy, framebuffer, red);
  // framebuffer.set(ax, ay, white);
  // framebuffer.set(bx, by, white);
  // framebuffer.set(cx, cy, white);

  std::srand(std::time({}));

  // readObjFile("african_head.obj", framebuffer);
  readObjFile("obj/african_head/african_head.obj", framebuffer);
  // for (int i = 0; i < (1 << 24); i++) {
  //   int ax = std::rand() % width;
  //   int ay = std::rand() % height;
  //   int bx = std::rand() % width;
  //   int by = std::rand() % height;
  //   // drawLine(ax, ay, bx, by, framebuffer,
  //   //          {static_cast<uint8_t>(static_cast<int>(std::rand() % 255)),
  //   //           static_cast<uint8_t>(static_cast<int>(std::rand() % 255)),
  //   //           static_cast<uint8_t>(static_cast<int>(std::rand() % 255)),
  //   //           static_cast<uint8_t>(static_cast<int>(std::rand() %
  //   //           255))});
  //   drawOptimizedLine(
  //       ax, ay, bx, by, framebuffer,
  //       {static_cast<uint8_t>(static_cast<int>(std::rand() % 255)),
  //        static_cast<uint8_t>(static_cast<int>(std::rand() % 255)),
  //        static_cast<uint8_t>(static_cast<int>(std::rand() % 255)),
  //        static_cast<uint8_t>(static_cast<int>(std::rand() % 255))});
  // }

  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
