#include <stdexcept>
#include "square.hpp"

topit::Square::Square(p_t p, int s):
  IDraw(),
  start(p),
  side(s)
  {
  if (side <= 0)
    throw std::invalid_argument("Invalid square side");
}

topit::p_t topit::Square::begin() const {
    return start;
}

topit::p_t topit::Square::next(p_t prev) const {
  if (prev.x < start.x || prev.x >= start.x + side 
    || prev.y < start.y || prev.y >= start.y + side)
  {
    throw std::logic_error("Square: a bad point");
  }
  if (prev.y == start.y && prev.x < start.x + side - 1) {
    return {prev.x + 1, prev.y};
  } else if (prev.x == start.x + side -1 && prev.y < start.y + side - 1) {
    return {prev.x, prev.y + 1};
  } else if (prev.y == start.y + side - 1 && prev.x > start.x) {
    return {prev.x - 1, prev.y};
  } else if (prev.x == start.x && prev.y > start.y) {
    return {prev.x, prev.y - 1};
  } else {
    return start;
  }
}