#pragma once

#include "../Image/tgaimage.h"
#include "../Utils/PerfStats.h"
#include <SDL3/SDL.h>
#include <memory>
#include <string>

namespace window {

namespace detail {

struct WindowDeleter {
  void operator()(SDL_Window *w) const noexcept { SDL_DestroyWindow(w); }
};
struct RendererDeleter {
  void operator()(SDL_Renderer *r) const noexcept { SDL_DestroyRenderer(r); }
};
struct TextureDeleter {
  void operator()(SDL_Texture *t) const noexcept { SDL_DestroyTexture(t); }
};

} // namespace detail

// Owns a single SDL window/renderer/texture and streams a TGAImage buffer to
// it, so a rasterization process can be watched as it happens rather than
// only inspected once written to disk. SDL must already be initialized
// (WindowManager owns that lifetime) before a Window is constructed, so
// several Windows - e.g. one per framebuffer - can coexist side by side.
class Window {
public:
  Window(int width, int height, const std::string &title);

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
  Window(Window &&) = default;
  Window &operator=(Window &&) = default;

  // Uploads the buffer's current contents and presents them.
  void present(const TGAImage &buffer);

  // Marks the window closed; it stops being presented to, though the
  // underlying SDL resources are only released when this Window is
  // destroyed. Called by WindowManager once it routes a close event here.
  void close();

  bool isOpen() const;

  // SDL's per-window id, used by WindowManager to route events (e.g. a
  // close request) to the right Window.
  SDL_WindowID id() const;

  // Rewrites the window title to "<base title> | Time: ..s | FPS: .."
  // whenever the throttled PerfStats says it's due for a refresh. Exposed
  // so a caller idling on this window (no new frames to present) can keep
  // the running time in the title ticking.
  void refreshTitle();

private:
  // Declaration order matters: members are destroyed in reverse order, so
  // texture_/renderer_ tear down before window_.
  std::unique_ptr<SDL_Window, detail::WindowDeleter> window_;
  std::unique_ptr<SDL_Renderer, detail::RendererDeleter> renderer_;
  std::unique_ptr<SDL_Texture, detail::TextureDeleter> texture_;
  bool open_ = false;
  std::string baseTitle_;
  utils::PerfStats stats_;
};

} // namespace window
