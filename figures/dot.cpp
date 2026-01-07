#include <stdexcept>
#include "dot.hpp"

topit::Dot::Dot(p_t dd):
  IDraw(),
  d{dd}
{}

topit::p_t topit::Dot::begin() const {
  return d;
}

topit::p_t topit::Dot::next(p_t prev) const {
  if (prev != d) {
    throw std::logic_error("Dot error: a bad previous point");
  }
  return d;
}