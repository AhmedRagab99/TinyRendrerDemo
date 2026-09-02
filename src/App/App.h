#pragma once

#include "../Image/tgaimage.h"
#include "../Model/model.h"
#include "../Models/Models.h"
#include "../Renderer/Renderer.h"
#include "../Window/WindowManager.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace App {

class Application {
public:
  void start();
  void loadModel(const std::string &filename, TGAImage &framebuffer);
  void addTriangle(const Models::Points3D &points, const glm::vec3 &normal,
                   TGAImage &framebuffer, TGAColor color);

private:
  // PROJECT_ROOT_DIR is injected by CMake so this resolves regardless of the
  // working directory the binary is launched from (see CMakeLists.txt).
  std::string modelPath_ =
      std::string(PROJECT_ROOT_DIR) + "/obj/african_head/african_head.obj";
  // std::string modelPath_ =
  //     std::string(PROJECT_ROOT_DIR) + "/obj/african_head/african_head.obj";
  std::string outputFile_ = "framebuffer.tga";
  int width_ = 800;
  int height_ = 800;
  std::unique_ptr<window::WindowManager> windowManager_;
};

} // namespace App
