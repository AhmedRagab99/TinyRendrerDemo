#include "WindowManager.h"

#include <iostream>

namespace window {

WindowManager::WindowManager(int width, int height, const char *title) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
    return;
  }

  window_ = SDL_CreateWindow(title, width, height, 0);
  if (!window_) {
    std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
    SDL_Quit();
    return;
  }

  renderer_ = SDL_CreateRenderer(window_, nullptr);
  if (!renderer_) {
    std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
    SDL_DestroyWindow(window_);
    SDL_Quit();
    window_ = nullptr;
    return;
  }

  texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGB24,
                               SDL_TEXTUREACCESS_STREAMING, width, height);
  if (!texture_) {
    std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << "\n";
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    renderer_ = nullptr;
    window_ = nullptr;
    return;
  }

  open_ = true;
}

WindowManager::~WindowManager() {
  if (texture_)
    SDL_DestroyTexture(texture_);
  if (renderer_)
    SDL_DestroyRenderer(renderer_);
  if (window_) {
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }
}

void WindowManager::present(const TGAImage &framebuffer) {
  if (!open_)
    return;

  int pitch = framebuffer.width() * framebuffer.bytesPerPixel();
  SDL_UpdateTexture(texture_, nullptr, framebuffer.buffer(), pitch);

  SDL_RenderClear(renderer_);
  // the framebuffer stores row 0 as the bottom of the image, so flip it
  // vertically to match the orientation of the final written .tga file
  SDL_RenderTextureRotated(renderer_, texture_, nullptr, nullptr, 0.0, nullptr,
                           SDL_FLIP_VERTICAL);
  SDL_RenderPresent(renderer_);
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
  }
}

} // namespace window
