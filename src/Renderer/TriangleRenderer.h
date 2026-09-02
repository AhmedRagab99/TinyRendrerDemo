#pragma once

#include "../Image/tgaimage.h"
#include "../Models/Models.h"
#include "LineRenderer.h"
#include "RenderTarget.h"

#include <algorithm>
#include <utility>

namespace renderer {

class TriangleRenderer {
  // sort points

public:
  static void sortPoints(Models::Points3D &point);
  static double signed_triangle_area(Models::Points3D &point);

  template <RenderTarget Target>
  static void fillTriangle(Models::Points3D &point, Target &framebuffer,
                           TGAColor color);

  template <RenderTarget Target>
  static void fillOptimizedTriangle(Models::Points3D &point,
                                    Target &framebuffer, TGAColor color);
};

// used to get the bounding box of the triangle then get the area of the
// triangle and then plot only the dots if its in the triangle
template <RenderTarget Target>
void TriangleRenderer::fillOptimizedTriangle(Models::Points3D &point,
                                             Target &framebuffer,
                                             TGAColor color) {

  int bbminx = std::min(std::min(point.ax, point.bx), point.cx);
  int bbminy = std::min(std::min(point.ay, point.by), point.cy);
  int bbmaxx = std::max(std::max(point.ax, point.bx), point.cx);
  int bbmaxy = std::max(std::max(point.ay, point.by), point.cy);
  double total_area = signed_triangle_area(point);

  // backface culling + discarding triangles that cover less than a pixel
  if (total_area == 0)
    return;

#pragma omp parallel for
  for (int y = bbminy; y <= bbmaxy; y++) {
    int xStart = -1;
    int xEnd = -1;

    for (int x = bbminx; x <= bbmaxx; x++) {
      Models::Points3D pbc(x, y, point.bx, point.by, point.cx, point.cy);
      Models::Points3D apc(point.ax, point.ay, x, y, point.cx, point.cy);
      Models::Points3D abp(point.ax, point.ay, point.bx, point.by, x, y);

      double alpha = signed_triangle_area(pbc) / total_area;
      double beta = signed_triangle_area(apc) / total_area;
      double gamma = signed_triangle_area(abp) / total_area;

      if (alpha >= 0 && beta >= 0 && gamma >= 0) {
        if (xStart == -1)
          xStart = x;
        xEnd = x;
      }
    }

    if (xStart != -1) {
      LineRenderer::drawOptimizedLine(xStart, y, xEnd, y, framebuffer, color);
    }
  }
}

template <RenderTarget Target>
void TriangleRenderer::fillTriangle(Models::Points3D &point,
                                    Target &framebuffer, TGAColor color) {

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

} // namespace renderer
