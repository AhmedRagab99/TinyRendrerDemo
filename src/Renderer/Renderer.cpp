#include "Renderer.h"
#include "LineRenderer.h"
#include "TriangleRenderer.h"

namespace renderer {

void Renderer::draw(Primitive type, Models::Points3D points,
                    TGAImage &framebuffer, TGAColor color, int thickness) {
  switch (type) {
  case Primitive::Line:
    LineRenderer::drawOptimizedLine(points.ax, points.ay, points.bx, points.by,
                                    framebuffer, color, thickness);
    break;
  case Primitive::Triangle:
    TriangleRenderer::fillTriangle(points, framebuffer, color);
    break;
  case Primitive::optimizedTriangle:
    TriangleRenderer::fillOptimizedTriangle(points, framebuffer, color);
    break;
  }
}

} // namespace renderer
