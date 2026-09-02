#pragma once

#include "../Image/tgaimage.h"
#include "Window.h"
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>

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

} // namespace detail

// Owns the SDL lifetime and a collection of independent, named Windows, so a
// renderer with several buffers (framebuffer, z-buffer, ...) can show each
// one in its own live-updating window instead of being limited to a single
// view. Windows are addressed by the id string passed to createWindow.
class WindowManager {
public:
  WindowManager();

  WindowManager(const WindowManager &) = delete;
  WindowManager &operator=(const WindowManager &) = delete;

  // Creates and registers a new window under `id`, replacing any existing
  // window already registered under that id. Returns the new window, or
  // nullptr if SDL failed to initialize or window creation failed.
  Window *createWindow(const std::string &id, int width, int height,
                       const std::string &title);

  // Looks up a previously created window; nullptr if `id` is unknown.
  Window *getWindow(const std::string &id);

  // Uploads and presents `buffer` in the window registered under `id`.
  // No-op if no such window exists or it isn't open.
  void present(const std::string &id, const TGAImage &buffer);

  // Pumps the SDL event queue once, routing each event to the window it
  // targets (or, for an application-wide quit, to every window).
  void pollEvents();

  // True once SDL initialized and at least one window is still open.
  bool isOpen() const;

  // Pumps events and blocks until every window has been closed.
  void waitUntilClosed();

private:
  detail::SdlInitGuard sdlInit_;
  std::unordered_map<std::string, std::unique_ptr<Window>> windows_;
};

} // namespace window
