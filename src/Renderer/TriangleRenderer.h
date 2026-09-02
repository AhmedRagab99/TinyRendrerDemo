#pragma once

#include "../Image/tgaimage.h"
#include "../Models/Models.h"
#include "FrameBuffers.h"
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
                                    FrameBuffers<Target> &buffers,
                                    TGAColor color);
};

// Gets the bounding box of the triangle, then for each pixel inside it
// computes barycentric weights to test membership and interpolate depth;
// only pixels closer than what's already in the z-buffer get written, so
// triangles drawn in any order still occlude correctly.
template <RenderTarget Target>
void TriangleRenderer::fillOptimizedTriangle(Models::Points3D &point,
                                             FrameBuffers<Target> &buffers,
                                             TGAColor color) {
  Target &framebuffer = buffers.color;
  Target &zbuffer = buffers.zbuffer;

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
    for (int x = bbminx; x <= bbmaxx; x++) {
      Models::Points3D pbc(x, y, 0, point.bx, point.by, 0, point.cx, point.cy,
                           0);
      Models::Points3D apc(point.ax, point.ay, 0, x, y, 0, point.cx, point.cy,
                           0);
      Models::Points3D abp(point.ax, point.ay, 0, point.bx, point.by, 0, x, y,
                           0);

      double alpha = signed_triangle_area(pbc) / total_area;
      double beta = signed_triangle_area(apc) / total_area;
      double gamma = signed_triangle_area(abp) / total_area;

      if (alpha < 0 || beta < 0 || gamma < 0)
        continue; // outside the triangle

      // depth at this pixel, interpolated from the triangle's own vertex
      // depths (not the pbc/apc/abp helper triangles above, which only
      // exist to get area ratios and carry no meaningful z of their own)
      unsigned char z = static_cast<unsigned char>(
          alpha * point.az + beta * point.bz + gamma * point.cz);
      if (z <= zbuffer.get(x, y)[0])
        continue; // occluded by something already drawn closer

      framebuffer.set(x, y, color);
      zbuffer.set(x, y, TGAColor{z, z, z, 255});
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
