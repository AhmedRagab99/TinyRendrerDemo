#include "WindowManager.h"

#include <iostream>

namespace window {

WindowManager::WindowManager() {
  if (!sdlInit_.ok())
    std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
}

Window *WindowManager::createWindow(const std::string &id, int width,
                                    int height, const std::string &title) {
  if (!sdlInit_.ok())
    return nullptr;

  auto window = std::make_unique<Window>(width, height, title);
  if (!window->isOpen())
    return nullptr;

  Window *raw = window.get();
  windows_[id] = std::move(window);
  return raw;
}

Window *WindowManager::getWindow(const std::string &id) {
  auto it = windows_.find(id);
  return it == windows_.end() ? nullptr : it->second.get();
}

void WindowManager::present(const std::string &id, const TGAImage &buffer) {
  if (Window *window = getWindow(id))
    window->present(buffer);
}

void WindowManager::pollEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      for (auto &[id, window] : windows_)
        window->close();
      continue;
    }

    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
      for (auto &[id, window] : windows_) {
        if (window->id() == event.window.windowID) {
          window->close();
          break;
        }
      }
    }
  }
}

bool WindowManager::isOpen() const {
  for (const auto &[id, window] : windows_) {
    if (window->isOpen())
      return true;
  }
  return false;
}

void WindowManager::waitUntilClosed() {
  while (isOpen()) {
    pollEvents();
    // No new frames are being rendered here, but keep each window's running
    // time ticking in its title while it sits idle.
    for (auto &[id, window] : windows_)
      window->refreshTitle();
  }
}

} // namespace window
