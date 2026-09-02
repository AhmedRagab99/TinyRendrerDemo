#pragma once

#include "../Image/tgaimage.h"
#include "../Model/model.h"
#include "../Models/Models.h"
#include "../Renderer/FrameBuffers.h"
#include "../Renderer/Renderer.h"
#include "../Window/WindowManager.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace App {

// Final: computes the whole model first, then shows the two finished
// buffers once. Animated: streams both buffers to their windows as each
// triangle is drawn, so the rasterization process can be watched live.
enum class RenderMode { Final, Animated };

class Application {
public:
  explicit Application(RenderMode mode = RenderMode::Animated);

  void start();
  void loadModel(const std::string &filename,
                 renderer::FrameBuffers<TGAImage> &buffers);
  void addTriangle(const Models::Points3D &points, const glm::vec3 &normal,
                   renderer::FrameBuffers<TGAImage> &buffers, TGAColor color);

private:
  // PROJECT_ROOT_DIR is injected by CMake so this resolves regardless of the
  // working directory the binary is launched from (see CMakeLists.txt).
  std::string modelPath_ =
      std::string(PROJECT_ROOT_DIR) + "/obj/diablo3_pose/diablo3_pose.obj";
  std::string outputFile_ = "framebuffer.tga";
  int width_ = 800;
  int height_ = 800;
  RenderMode mode_;
  std::unique_ptr<window::WindowManager> windowManager_;
};

} // namespace App
