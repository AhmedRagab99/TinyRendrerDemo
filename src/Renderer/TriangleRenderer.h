#pragma once

#include "../Models.h"
#include "../tgaimage.h"
namespace renderer {

class TriangleRenderer {
  // sort points

public:
  static void sortPoints(Models::Points3D &point);
  static void fillTriangle(Models::Points3D &point, TGAImage &framebuffer,
                           TGAColor color);
};

} // namespace renderer
