#ifndef IDRAW_HPP
#define IDRAW_HPP
#include "geom.hpp"

namespace topit {
  struct Dot : IDraw {
    Dot(p_t dd);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    p_t d;
  };
}

#endif