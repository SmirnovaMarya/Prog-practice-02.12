#include <stdexcept>
#include "frect.hpp"
#include "geom.hpp"

topit::FRect::FRect(p_t pos, int w, int h): // левый нижний угол прямоугольника
  IDraw(),
  rect{pos, {pos.x + w, pos.y + h}}
{
  if (!(w > 0 && h > 0)) {
    throw std::logic_error("FRect error: a rectangle cannot have zero or negative width/height");
  }
}

topit::FRect::FRect(p_t a, p_t b):
  FRect(a, b.x - a.x, b.y - a.y)
{}

topit::p_t topit::FRect::begin() const {
  return rect.aa;
}

topit::p_t topit::FRect::next(p_t prev) const {
  if (prev.x < rect.bb.x) {
    return {prev.x + 1, prev.y};
  } else if (prev.x == rect.bb.x && prev.y < rect.bb.y) {
    return {rect.aa.x, prev.y + 1};
  } else if (prev == rect.bb) {
    return rect.aa;
  }
  throw std::logic_error("FRect error: point is outside the rectangle");
}