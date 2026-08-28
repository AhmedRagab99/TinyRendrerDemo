#include "LineRenderer.h"
#include <cmath>

namespace renderer {

void LineRenderer::setThickPixel(int x, int y, TGAImage &framebuffer,
                                  TGAColor color, int thickness) {
  int half = thickness / 2;
  for (int dy = -half; dy <= half; dy++) {
    for (int dx = -half; dx <= half; dx++) {
      framebuffer.set(x + dx, y + dy, color);
    }
  }
}

void LineRenderer::drawOptimizedLine(int ax, int ay, int bx, int by,
                                      TGAImage &framebuffer, TGAColor color,
                                      int thickness) {
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
      setThickPixel(y, x, framebuffer, color, thickness);
    else
      setThickPixel(x, y, framebuffer, color, thickness);
    ierror += 2 * std::abs(by - ay);
    y += (by > ay ? 1 : -1) * (ierror > bx - ax);
    ierror -= 2 * (bx - ax) * (ierror > bx - ax);
  }
}

} // namespace renderer
