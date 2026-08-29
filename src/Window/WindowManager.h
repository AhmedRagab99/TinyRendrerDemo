#pragma once

#include "../Image/tgaimage.h"
#include <SDL3/SDL.h>

namespace window {

// Opens a live SDL3 preview window and streams a TGAImage framebuffer to it,
// so the rasterization process can be watched as it happens rather than only
// inspected once written to disk.
class WindowManager {
public:
  WindowManager(int width, int height, const char *title);
  ~WindowManager();

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
  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;
  SDL_Texture *texture_ = nullptr;
  bool open_ = false;
};

} // namespace window
