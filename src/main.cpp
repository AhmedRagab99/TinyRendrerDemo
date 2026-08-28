#include "tgaimage.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

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

int main(int argc, char **argv) {
  constexpr int width = 64;
  constexpr int height = 64;
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
  for (int i = 0; i < (1 << 24); i++) {
    int ax = std::rand() % width;
    int ay = std::rand() % height;
    int bx = std::rand() % width;
    int by = std::rand() % height;
    drawLine(ax, ay, bx, by, framebuffer,
             {static_cast<uint8_t>(static_cast<int>(std::rand() % 255)),
              static_cast<uint8_t>(static_cast<int>(std::rand() % 255)),
              static_cast<uint8_t>(static_cast<int>(std::rand() % 255)),
              static_cast<uint8_t>(static_cast<int>(std::rand() % 255))});
  }

  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
