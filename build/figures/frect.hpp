#ifndef FRECT_HPP
#define FRECT_HPP
#include "geom.hpp"
#include "idraw.hpp"

namespace topit {
  struct FRect : IDraw {
		FRect(p_t pos, int w, int h);
		FRect(p_t a, p_t b);
    f_t rect;
    p_t begin() const override;
    p_t next(p_t prev) const override;
  };
  void append(const IDraw* sh, p_t** ppts, size_t& s);
}

#endif