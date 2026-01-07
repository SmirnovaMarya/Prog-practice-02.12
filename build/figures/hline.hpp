#ifndef HLINE_HPP
#define HLINE_HPP
#include "geom.hpp"
#include "idraw.hpp"

namespace topit {
  struct VLine : IDraw {
  VLine(p_t start, int length);
  p_t begin() const override;
  p_t next(p_t prev) const override;
  int len;
  p_t start_point;
  };
}

#endif