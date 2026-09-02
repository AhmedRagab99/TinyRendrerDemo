#include "WindowManager.h"

#include <iostream>

namespace window {

WindowManager::WindowManager(int width, int height, const char *title)
    : baseTitle_(title) {
  if (!sdlInit_.ok()) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
    return;
  }

  window_.reset(SDL_CreateWindow(title, width, height, 0));
  if (!window_) {
    std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
    return;
  }

  renderer_.reset(SDL_CreateRenderer(window_.get(), nullptr));
  if (!renderer_) {
    std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
    return;
  }

  texture_.reset(SDL_CreateTexture(renderer_.get(), SDL_PIXELFORMAT_RGB24,
                                   SDL_TEXTUREACCESS_STREAMING, width, height));
  if (!texture_) {
    std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << "\n";
    return;
  }

  open_ = true;
}

void WindowManager::present(const TGAImage &framebuffer) {
  if (!open_)
    return;

  int pitch = framebuffer.width() * framebuffer.bytesPerPixel();
  SDL_UpdateTexture(texture_.get(), nullptr, framebuffer.buffer(), pitch);

  SDL_RenderClear(renderer_.get());
  // the framebuffer stores row 0 as the bottom of the image, so flip it
  // vertically to match the orientation of the final written .tga file
  SDL_RenderTextureRotated(renderer_.get(), texture_.get(), nullptr, nullptr,
                           0.0, nullptr, SDL_FLIP_VERTICAL);
  SDL_RenderPresent(renderer_.get());

  stats_.frame();
  refreshTitle();
}

void WindowManager::refreshTitle() {
  if (!open_ || !stats_.shouldRefreshDisplay())
    return;

  std::string title = baseTitle_ + " | " + stats_.toString();
  SDL_SetWindowTitle(window_.get(), title.c_str());
}

void WindowManager::pollEvents() {
  if (!open_)
    return;

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT)
      open_ = false;
  }
}

bool WindowManager::isOpen() const { return open_; }

void WindowManager::waitUntilClosed() {
  while (open_) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT)
        open_ = false;
    }
    // No new frames are being rendered here, but keep the running time in
    // the title ticking while the window sits idle.
    refreshTitle();
  }
}

} // namespace window
