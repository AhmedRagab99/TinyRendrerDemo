#pragma once
namespace Models {

struct Points3D {
  int ax, ay;
  int bx, by;
  int cx, cy;

  Points3D() = default;
  Points3D(int ax, int ay, int bx, int by, int cx, int cy)
      : ax(ax), ay(ay), bx(bx), by(by), cx(cx), cy(cy) {}
};

} // namespace Models
