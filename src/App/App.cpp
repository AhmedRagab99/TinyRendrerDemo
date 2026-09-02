#include "App.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace App {

namespace {
constexpr const char *kFramebufferWindowId = "framebuffer";
constexpr const char *kZBufferWindowId = "zbuffer";
} // namespace

Application::Application(RenderMode mode) : mode_(mode) {}

void Application::start() {
  renderer::FrameBuffers<TGAImage> buffers{
      .color = TGAImage(width_, height_, TGAImage::RGB),
      .zbuffer = TGAImage(width_, height_, TGAImage::GRAYSCALE)};

  std::srand(std::time({}));

  windowManager_ = std::make_unique<window::WindowManager>();

  if (mode_ == RenderMode::Animated) {
    // Windows are created up front so the process streams into them live.
    windowManager_->createWindow(kFramebufferWindowId, width_, height_,
                                 "TinyRenderer - Framebuffer");
    windowManager_->createWindow(kZBufferWindowId, width_, height_,
                                 "TinyRenderer - Z-Buffer");
  }

  loadModel(modelPath_, buffers);

  if (mode_ == RenderMode::Final) {
    // Windows only appear once the finished buffers are ready to show.
    windowManager_->createWindow(kFramebufferWindowId, width_, height_,
                                 "TinyRenderer - Framebuffer (Final)");
    windowManager_->createWindow(kZBufferWindowId, width_, height_,
                                 "TinyRenderer - Z-Buffer (Final)");
  }

  if (windowManager_->isOpen()) {
    windowManager_->present(kFramebufferWindowId, buffers.color);
    windowManager_->present(kZBufferWindowId, buffers.zbuffer);
    windowManager_->waitUntilClosed();
  }

  if (auto result = buffers.color.write_tga_file(outputFile_); !result) {
    std::cerr << "Failed to write framebuffer to " << outputFile_ << " (error "
              << static_cast<int>(result.error()) << ")" << std::endl;
  }
}

void Application::addTriangle(const Models::Points3D &points,
                              const glm::vec3 &normal,
                              renderer::FrameBuffers<TGAImage> &buffers,
                              TGAColor color) {
  // back-face culling: a triangle only faces the viewer (looking down -z)
  // when its normal has a positive z component
  if (normal.z <= 0.0f)
    return;

  renderer::Renderer::draw(renderer::Primitive::optimizedTriangle, points,
                           buffers, color);
}

void Application::loadModel(const std::string &filename,
                            renderer::FrameBuffers<TGAImage> &buffers) {
  auto modelResult = model::Model::load(filename);
  if (!modelResult) {
    std::cerr << "Failed to load model: " << filename << std::endl;
    return;
  }
  model::Model &mdl = *modelResult;
  std::cout << "Number of vertices: " << mdl.nverts() << std::endl;
  std::cout << "Number of faces: " << mdl.nfaces() << std::endl;

  bool animated = mode_ == RenderMode::Final;

  for (int i = 0; i < mdl.nfaces(); i++) {
    if (animated) {
      if (!windowManager_->isOpen())
        break;
      windowManager_->pollEvents();
    }

    std::vector<int> face = mdl.face(i);
    glm::vec3 v0 = mdl.vert(face[0]);
    glm::vec3 v1 = mdl.vert(face[1]);
    glm::vec3 v2 = mdl.vert(face[2]);
    // used for get the normalize version for each obj model
    glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);

    int x0 = static_cast<int>((v0.x + 1.0f) * buffers.width() / 2.0f);
    int y0 = static_cast<int>((v0.y + 1.0f) * buffers.height() / 2.0f);
    int x1 = static_cast<int>((v1.x + 1.0f) * buffers.width() / 2.0f);
    int y1 = static_cast<int>((v1.y + 1.0f) * buffers.height() / 2.0f);
    int x2 = static_cast<int>((v2.x + 1.0f) * buffers.width() / 2.0f);
    int y2 = static_cast<int>((v2.y + 1.0f) * buffers.height() / 2.0f);

    // depth mapped into the z-buffer's [0, 255] grayscale range, the same
    // way x/y are mapped into the buffer's pixel dimensions above
    int z0 = static_cast<int>((v0.z + 1.0f) * 255.0f / 2.0f);
    int z1 = static_cast<int>((v1.z + 1.0f) * 255.0f / 2.0f);
    int z2 = static_cast<int>((v2.z + 1.0f) * 255.0f / 2.0f);

    TGAColor faceColor = {static_cast<uint8_t>(std::rand() % 255),
                          static_cast<uint8_t>(std::rand() % 255),
                          static_cast<uint8_t>(std::rand() % 255), 255};

    addTriangle(Models::Points3D{x0, y0, z0, x1, y1, z1, x2, y2, z2}, normal,
                buffers, faceColor);

    if (animated) {
      windowManager_->present(kFramebufferWindowId, buffers.color);
      windowManager_->present(kZBufferWindowId, buffers.zbuffer);
    }
  }
}

} // namespace App
