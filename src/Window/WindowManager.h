#pragma once

#include "../Image/tgaimage.h"
#include "../Utils/PerfStats.h"
#include <SDL3/SDL.h>
#include <memory>
#include <string>

namespace window {

namespace detail {

// Pairs SDL_Init/SDL_Quit into one RAII object so SDL_Quit runs exactly once
// iff SDL_Init succeeded, no matter which later step (window/renderer/texture
// creation) fails and triggers early return from the constructor.
class SdlInitGuard {
public:
  SdlInitGuard() : initialized_(SDL_Init(SDL_INIT_VIDEO)) {}
  ~SdlInitGuard() {
    if (initialized_)
      SDL_Quit();
  }

  SdlInitGuard(const SdlInitGuard &) = delete;
  SdlInitGuard &operator=(const SdlInitGuard &) = delete;

  bool ok() const { return initialized_; }

private:
  bool initialized_;
};

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

// Opens a live SDL3 preview window and streams a TGAImage framebuffer to it,
// so the rasterization process can be watched as it happens rather than only
// inspected once written to disk.
class WindowManager {
public:
  WindowManager(int width, int height, const char *title);

  WindowManager(const WindowManager &) = delete;
  WindowManager &operator=(const WindowManager &) = delete;

  // Uploads the framebuffer's current contents and presents them.
  void present(const TGAImage &framebuffer);

  // Pumps the SDL event queue; closes the window on a quit request.
  void pollEvents();

  bool isOpen() const;

  // Pumps events and blocks until the window is closed.
  void waitUntilClosed();

private:
  // Rewrites the window title to "<base title> | Time: ..s | FPS: .."
  // whenever the throttled PerfStats says it's due for a refresh.
  void refreshTitle();

  // Declaration order matters: members are destroyed in reverse order, so
  // texture_/renderer_/window_ tear down before sdlInit_ calls SDL_Quit().
  detail::SdlInitGuard sdlInit_;
  std::unique_ptr<SDL_Window, detail::WindowDeleter> window_;
  std::unique_ptr<SDL_Renderer, detail::RendererDeleter> renderer_;
  std::unique_ptr<SDL_Texture, detail::TextureDeleter> texture_;
  bool open_ = false;
  std::string baseTitle_;
  utils::PerfStats stats_;
};

} // namespace window
