#pragma once

#include "../Image/tgaimage.h"
#include "../Models/Models.h"
#include "LineRenderer.h"
#include "RenderTarget.h"
#include "TriangleRenderer.h"

namespace renderer {

enum class Primitive { Line, Triangle, optimizedTriangle };

class Renderer {
public:
  template <RenderTarget Target>
  static void draw(Primitive type, Models::Points3D points,
                   Target &framebuffer, TGAColor color, int thickness = 0);
};

template <RenderTarget Target>
void Renderer::draw(Primitive type, Models::Points3D points,
                    Target &framebuffer, TGAColor color, int thickness) {
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
