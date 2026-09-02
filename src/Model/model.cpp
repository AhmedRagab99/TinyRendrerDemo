
#include "model.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace model {

std::expected<Model, ModelError> Model::load(const std::string &filename) {

  // add cout in all the functions to see if they are being called
  std::ifstream in;
  in.open(filename, std::ifstream::in);
  if (in.fail()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return std::unexpected(ModelError::FileOpenFailed);
  }

  Model model;
  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    std::istringstream iss(line.c_str());

    char trash;
    if (!line.compare(0, 2, "v ")) {
      iss >> trash;
      glm::vec3 v;
      for (int i = 0; i < 3; i++)
        iss >> v[i];
      model.verts_.push_back(v);
    } else if (!line.compare(0, 3, "vn ")) {
      iss >> trash >> trash;
      glm::vec3 n;
      for (int i = 0; i < 3; i++)
        iss >> n[i];
      model.norms_.push_back(n);
    } else if (!line.compare(0, 2, "f ")) {
      std::vector<int> f;
      int itrash, idx;
      iss >> trash;

      while (iss >> idx >> trash >> itrash >> trash >> itrash) {
        idx--; // in wavefront obj all indices start at 1, not zero
        f.push_back(idx);
      }
      model.faces_.push_back(f);
    }
  }
  std::cerr << "# v# " << model.verts_.size() << " f# " << model.faces_.size()
            << " vt# " << model.norms_.size() << std::endl;

  return model;
}

int Model::nverts() const { return (int)verts_.size(); }
int Model::nfaces() const { return (int)faces_.size(); }
glm::vec3 Model::vert(int i) const { return verts_[i]; }
glm::vec3 Model::normal(int i) const { return norms_[i]; }
std::vector<int> Model::face(int idx) const { return faces_[idx]; }

} // namespace model
