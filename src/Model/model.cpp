
#include "model.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace model {

Model::Model(const std::string &filename) {

  // add cout in all the functions to see if they are being called
  std::ifstream in;
  in.open(filename, std::ifstream::in);
  if (in.fail()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }
  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    std::istringstream iss(line.c_str());

    char trash;
    if (!line.compare(0, 2, "v ")) {
      iss >> trash;
      glm::vec3 v;
      std::cerr << "Reading vertex: " << line << std::endl;
      for (int i = 0; i < 3; i++)
        iss >> v[i];
      verts_.push_back(v);
    } else if (!line.compare(0, 3, "vn ")) {
      iss >> trash >> trash;
      glm::vec3 n;
      std::cerr << "Reading normal: " << line << std::endl;
      for (int i = 0; i < 3; i++)
        iss >> n[i];
      norms_.push_back(n);
    } else if (!line.compare(0, 2, "f ")) {
      std::vector<int> f;
      int itrash, idx;
      iss >> trash;
      std::cerr << "Reading face: " << line << std::endl;
      while (iss >> idx >> trash >> itrash >> trash >> itrash) {
        idx--; // in wavefront obj all indices start at 1, not zero
        f.push_back(idx);
      }
      faces_.push_back(f);
    }
  }
  std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# "
            << norms_.size() << std::endl;
}
int Model::nverts() { return (int)verts_.size(); }
int Model::nfaces() { return (int)faces_.size(); }
glm::vec3 Model::vert(const int i) { return verts_[i]; }
glm::vec3 Model::normal(int i) { return norms_[i]; }
std::vector<int> Model::face(int idx) { return faces_[idx]; }

} // namespace model
