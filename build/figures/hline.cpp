#include <stdexcept>
#include "hline.hpp"

topit::VLine::VLine(p_t start, int length):
  IDraw(),
  start_point(start),
  len(length)
  {
    if (len <= 0) {
      throw std::logic_error("Length must be > 0");
    }
  }

topit::p_t topit::VLine::begin() const {
  return start_point;
}

topit::p_t topit::VLine::next(p_t prev) const {
  if (prev.x != start_point.x) {
    throw std::logic_error("VLine error: bad x");
  }
  int idx = prev.y - start_point.y;
  if (idx < 0 || static_cast<size_t>(idx) >= len) {
    throw std::logic_error("VLine error: point outside line");
  }
  if (static_cast<size_t>(idx + 1) < len) {
    return {start_point.x, prev.y + 1};
  }
  return start_point;
}