#include "PerfStats.h"

#include <iomanip>
#include <sstream>

namespace utils {

PerfStats::PerfStats()
    : start_(clock::now()), lastRefresh_(start_), fpsWindowStart_(start_) {}

void PerfStats::frame() {
  ++framesSinceFpsWindow_;

  const auto now = clock::now();
  const auto sinceFpsWindow = now - fpsWindowStart_;
  if (sinceFpsWindow >= kFpsWindow) {
    fps_ = framesSinceFpsWindow_ /
           std::chrono::duration<double>(sinceFpsWindow).count();
    framesSinceFpsWindow_ = 0;
    fpsWindowStart_ = now;
  }
}

bool PerfStats::shouldRefreshDisplay() {
  const auto now = clock::now();
  if (now - lastRefresh_ < kRefreshInterval)
    return false;

  lastRefresh_ = now;
  return true;
}

double PerfStats::elapsedSeconds() const {
  return std::chrono::duration<double>(clock::now() - start_).count();
}

double PerfStats::fps() const { return fps_; }

std::string PerfStats::toString() const {
  std::ostringstream oss;
  oss << "Time: " << std::fixed << std::setprecision(1) << elapsedSeconds()
      << "s | FPS: " << std::fixed << std::setprecision(0) << fps_;
  return oss.str();
}

} // namespace utils
