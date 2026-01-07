#include <stdexcept>
#include "rect.hpp"
#include "geom.hpp"

topit::Rect::Rect(p_t pos, int w, int h): // левый нижний угол, ширина, высота
  IDraw(),
  rect{pos, {pos.x + w, pos.y + h}}
{
  if (!(w > 0 && h > 0)) {
    throw std::logic_error("Rect error: a rectangle cannot have zero or negative width/height");
  }
}

topit::Rect::Rect(p_t a, p_t b):
  Rect(a, b.x - a.x, b.y - a.y)
{}

topit::p_t topit::Rect::begin() const {
  return rect.aa;
}

topit::p_t topit::Rect::next(p_t prev) const {
  if (prev.x == rect.aa.x && prev.y < rect.bb.y) { // левая сторона прямоугольника
    return {prev.x, prev.y + 1};
  } else if (prev.y == rect.bb.y && prev.x < rect.bb.x) { // верхняя сторона прямоугольника
    return {prev.x + 1, prev.y};
  } else if (prev.x == rect.bb.x && prev.y > rect.aa.y) { // правая сторона прямоугольника
    return {prev.x, prev.y - 1};
  } else if (prev.y == rect.aa.y && prev.x > rect.aa.x) {
    return {prev.x - 1, prev.y}; // нижняя сторона прямоугольника
  }
  throw std::logic_error("Rect error: point is outside the rectangle");
}