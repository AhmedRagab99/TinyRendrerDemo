#include "TriangleRenderer.h"
#include "LineRenderer.h"

#include <algorithm>
#include <utility>
namespace renderer {

void TriangleRenderer::fillTriangle(Models::Points3D &point,
                                    TGAImage &framebuffer, TGAColor color) {

  // sort the points
  sortPoints(point);
  // get the lower segment
  int total_height = point.cy - point.ay;
  if (total_height == 0)
    return;

  for (int y = point.ay; y <= point.cy; y++) {
    bool lower_segment = y > point.by || point.by == point.ay;
    int segment_height =
        lower_segment ? (point.cy - point.by) : (point.by - point.ay);
    if (segment_height == 0)
      continue;

    float alpha = (float)(y - point.ay) / total_height;
    float beta = lower_segment ? (float)(y - point.by) / segment_height
                               : (float)(y - point.ay) / segment_height;

    int xLong = point.ax + (int)((point.cx - point.ax) * alpha);
    int xShort = lower_segment ? point.bx + (int)((point.cx - point.bx) * beta)
                               : point.ax + (int)((point.bx - point.ax) * beta);

    if (xLong > xShort)
      std::swap(xLong, xShort);

    LineRenderer::drawOptimizedLine(xLong, y, xShort, y, framebuffer, color);
  }
}

void TriangleRenderer::sortPoints(
    Models::Points3D &point) { // sort points based on the y

  /// sort based on the a and b values
  /// for the ab line
  if (point.ay > point.by) {
    std::swap(point.ax, point.bx);
    std::swap(point.ay, point.by);
  }

  // for the ac line
  if (point.ay > point.cy) {
    std::swap(point.ax, point.cx);
    std::swap(point.ay, point.cy);
  }

  if (point.by > point.cy) {
    std::swap(point.bx, point.cx);
    std::swap(point.by, point.cy);
  }
}

} // namespace renderer
