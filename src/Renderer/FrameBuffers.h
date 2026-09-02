#pragma once

#include "RenderTarget.h"

namespace renderer {

// Bundles every buffer a draw call writes into. Renderer/TriangleRenderer
// and App take this instead of a separate parameter per buffer, so adding
// another one later (a normal buffer, an object-id buffer, ...) only means
// adding a field here rather than threading a new parameter through every
// draw call and its callers.
template <RenderTarget Target> struct FrameBuffers {
  Target color;
  Target zbuffer;

  int width() const { return color.width(); }
  int height() const { return color.height(); }
};

} // namespace renderer
