#include "Window.h"

#include <iostream>
#include <vector>

namespace window {

Window::Window(int width, int height, const std::string &title)
    : baseTitle_(title) {
  window_.reset(SDL_CreateWindow(title.c_str(), width, height, 0));
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

void Window::present(const TGAImage &buffer) {
  if (!open_)
    return;

  int w = buffer.width();
  int h = buffer.height();
  int bpp = buffer.bytesPerPixel();
  int pitch = w * 3;

  if (bpp == 3) {
    SDL_UpdateTexture(texture_.get(), nullptr, buffer.buffer(), pitch);
  } else {
    // The texture is fixed at RGB24, but buffers can be single-channel
    // (e.g. a z-buffer) or carry an alpha channel, so re-pack into RGB24
    // here rather than tying the texture format to whichever buffer
    // happens to be presented.
    std::vector<std::uint8_t> rgb(static_cast<size_t>(w) * h * 3);
    const std::uint8_t *src = buffer.buffer();
    for (int i = 0; i < w * h; ++i) {
      const std::uint8_t *p = src + i * bpp;
      rgb[i * 3 + 0] = p[0];
      rgb[i * 3 + 1] = bpp >= 2 ? p[1] : p[0];
      rgb[i * 3 + 2] = bpp >= 3 ? p[2] : p[0];
    }
    SDL_UpdateTexture(texture_.get(), nullptr, rgb.data(), pitch);
  }

  SDL_RenderClear(renderer_.get());
  // the framebuffer stores row 0 as the bottom of the image, so flip it
  // vertically to match the orientation of the final written .tga file
  SDL_RenderTextureRotated(renderer_.get(), texture_.get(), nullptr, nullptr,
                           0.0, nullptr, SDL_FLIP_VERTICAL);
  SDL_RenderPresent(renderer_.get());

  stats_.frame();
  refreshTitle();
}

void Window::refreshTitle() {
  if (!open_ || !stats_.shouldRefreshDisplay())
    return;

  std::string title = baseTitle_ + " | " + stats_.toString();
  SDL_SetWindowTitle(window_.get(), title.c_str());
}

void Window::close() { open_ = false; }

bool Window::isOpen() const { return open_; }

SDL_WindowID Window::id() const {
  return window_ ? SDL_GetWindowID(window_.get()) : 0;
}

} // namespace window
