#ifndef SQUARE_HPP
#define SQUARE_HPP
#include "geom.hpp"
#include "idraw.hpp"

namespace topit {
  struct Square : IDraw {
    Square(p_t p, int s);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    p_t start;
    int side;
  };
}

#endif