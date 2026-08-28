#pragma once

#include "../Image/tgaimage.h"
#include "../Models/Models.h"

namespace renderer {

enum class Primitive { Line, Triangle };

class Renderer {
public:
  static void draw(Primitive type, Models::Points3D points,
                    TGAImage &framebuffer, TGAColor color,
                    int thickness = 0);
};

} // namespace renderer
