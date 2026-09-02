#pragma once

#include "../Image/tgaimage.h"
#include <concepts>

namespace renderer {

// Anything a rasterizer can write pixels into: a real TGAImage, a headless
// test double, an SDL-texture-backed buffer, etc. Renderer/LineRenderer/
// TriangleRenderer are templated on this instead of hardcoding TGAImage, so
// swapping the target - e.g. a fake buffer in a unit test - needs no changes
// to the rasterization code itself, only a type that satisfies this shape.
template <typename T>
concept RenderTarget = requires(T &t, int x, int y, const TGAColor &c) {
  { t.set(x, y, c) } -> std::same_as<void>;
  { t.get(x, y) } -> std::convertible_to<TGAColor>;
  { t.width() } -> std::convertible_to<int>;
  { t.height() } -> std::convertible_to<int>;
};

} // namespace renderer
