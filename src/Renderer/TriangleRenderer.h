#pragma once

#include "../Image/tgaimage.h"
#include "../Models/Models.h"
namespace renderer {

class TriangleRenderer {
  // sort points

public:
  static void sortPoints(Models::Points3D &point);
  static void fillTriangle(Models::Points3D &point, TGAImage &framebuffer,
                           TGAColor color);
};

} // namespace renderer
