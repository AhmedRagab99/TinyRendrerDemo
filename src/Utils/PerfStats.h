#pragma once

#include <chrono>
#include <string>

namespace utils {

// Tracks total running time and frames-per-second, and throttles how often a
// caller should refresh a display (e.g. a window title) with them so it
// isn't rewritten on every single frame/loop iteration.
class PerfStats {
public:
  PerfStats();

  // Call once per rendered frame to feed the FPS calculation.
  void frame();

  // Call as often as you like; returns true (at most once every
  // kRefreshInterval) when it's time to redraw the stats display.
  bool shouldRefreshDisplay();

  double elapsedSeconds() const;
  double fps() const;

  // e.g. "Time: 12.3s | FPS: 60"
  std::string toString() const;

private:
  using clock = std::chrono::steady_clock;

  clock::time_point start_;
  clock::time_point lastRefresh_;
  clock::time_point fpsWindowStart_;
  int framesSinceFpsWindow_ = 0;
  double fps_ = 0.0;

  static constexpr std::chrono::milliseconds kRefreshInterval{200};
  static constexpr std::chrono::milliseconds kFpsWindow{500};
};

} // namespace utils
