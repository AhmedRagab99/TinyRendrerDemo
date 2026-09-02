#pragma once
namespace Models {

struct Points3D {
  int ax, ay, az;
  int bx, by, bz;
  int cx, cy, cz;

  Points3D() = default;
  Points3D(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy,
           int cz)
      : ax(ax), ay(ay), az(az), bx(bx), by(by), bz(bz), cx(cx), cy(cy),
        cz(cz) {}
};

} // namespace Models
