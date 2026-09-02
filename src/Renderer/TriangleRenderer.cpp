#include "TriangleRenderer.h"

#include <utility>
namespace renderer {

double TriangleRenderer::signed_triangle_area(Models::Points3D &point) {
  return .5 * ((point.by - point.ay) * (point.bx + point.ax) +
               (point.cy - point.by) * (point.cx + point.bx) +
               (point.ay - point.cy) * (point.ax + point.cx));
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
