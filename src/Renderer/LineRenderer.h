#pragma once

#include "../Image/tgaimage.h"

namespace renderer {

class LineRenderer {
public:
  static void drawOptimizedLine(int ax, int ay, int bx, int by,
                                 TGAImage &framebuffer, TGAColor color,
                                 int thickness = 0);

private:
  static void setThickPixel(int x, int y, TGAImage &framebuffer,
                             TGAColor color, int thickness);
};

} // namespace renderer
