#ifndef IDRAW_HPP
#define IDRAW_HPP
#include "geom.hpp"

namespace topit {
  struct IDraw {
      virtual ~IDraw() = default;
      virtual p_t begin() const = 0;
      virtual p_t next(p_t prev) const = 0;
  };
  p_t* extend(const p_t* pts, size_t s, p_t fill);
  void extend(p_t** pts, size_t& s, p_t fill);
  void append(const IDraw* sh, p_t** ppts, size_t& s);
}

#endif