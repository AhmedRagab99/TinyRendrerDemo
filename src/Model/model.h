#include <glm/glm.hpp>

class Model {
  std::vector<glm::vec3> verts_;
  std::vector<glm::vec3> norms_;
  std::vector<std::vector<int>> faces_; // list of indices for each face

public:
  Model(const std::string &filename);
  int nverts(); // number of vertices
  int nfaces(); // number of faces or triangles
  glm::vec3 vert(int i);
  glm::vec3 normal(int i);
  std::vector<int> face(int idx);
  glm::vec3 vert(const int i) const;   // 0 <= i < nverts()
  glm::vec3 normal(const int i) const; // 0 <= i < nverts()
};
