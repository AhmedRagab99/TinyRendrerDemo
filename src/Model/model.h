#pragma once

#include <expected>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace model {

enum class ModelError { FileOpenFailed };

class Model {
public:
  [[nodiscard]] static std::expected<Model, ModelError>
  load(const std::string &filename);

  int nverts() const;                  // number of vertices
  int nfaces() const;                  // number of faces or triangles
  glm::vec3 vert(int i) const;         // 0 <= i < nverts()
  glm::vec3 normal(int i) const;       // 0 <= i < nverts()
  std::vector<int> face(int idx) const;

private:
  Model() = default;

  std::vector<glm::vec3> verts_;
  std::vector<glm::vec3> norms_;
  std::vector<std::vector<int>> faces_; // list of indices for each face
};

} // namespace model
