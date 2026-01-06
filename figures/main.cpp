#include <iostream>
#include <stdexcept>
#include <algorithm>
namespace topit {
  struct p_t {
    int x, y;
  };
  struct f_t {
    p_t aa, bb;
  };
  size_t rows(f_t fr);
  size_t cols(f_t fr);
  bool operator==(p_t a, p_t b);
  bool operator!=(p_t a, p_t b);
  struct IDraw {
    virtual ~IDraw() = default;
    virtual p_t begin() const = 0;
    virtual p_t next(p_t prev) const = 0;
  };
  struct Dot: IDraw {
    Dot(p_t dd);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    p_t d;
  };
  struct Rect: IDraw {
    Rect(p_t pos, int w, int h);
    Rect(p_t a, p_t b);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    f_t rect;
  };
  struct FRect : IDraw {
		FRect(p_t pos, int w, int h);
		FRect(p_t a, p_t b);
    f_t rect;
    p_t begin() const override;
    p_t next(p_t prev) const override;
};
  p_t* extend(const p_t* pts, size_t s, p_t fill);
  void extend(p_t** pts, size_t& s, p_t fill);
  void append(const IDraw* sh, p_t** ppts, size_t& s);
  f_t frame(const p_t * pts, size_t s);
  char * canvas(f_t fr, char fill);
  void paint(p_t p, char* cnv, f_t fr, char fill);
  void flush(std::ostream& os, const char* cnv, f_t fr);
  struct Layers {
    Layers();
    ~Layers();
    Layers(const Layers&);
    Layers(Layers&&) noexcept;
    Layers& operator=(const Layers& other);
    Layers& operator=(Layers&&) noexcept;

    void append(const IDraw & dr);
    f_t frame() const {
      return topit::frame(pts_, points_);
    }
    size_t points() const {
      return points_;
    }
    size_t layers() const {
      return layers_;
    }
    size_t start(size_t i) const {
      if (i == 0)
        return 0;
      else
        return sizes_[i-1];
    }
    size_t end(size_t i) const {
      return sizes_[i];
    }
    p_t point(size_t i) const {
      return pts_[i];
    }
    private:
      size_t points_, layers_;
      p_t * pts_;
      size_t * sizes_;
  };
	f_t frame(const Layers& ls);
}

void topit::Layers::append(const IDraw& dr) { // dr - фигура
  size_t* ext_sizes = new size_t[layers_+1];
  try {
    topit::append(&dr, &pts_, points_); // берёт все точки фигуры dr и добавляет их в общий массив pts_
  } catch (...) {
    delete [] ext_sizes;
    throw;
  }
  // скопировать старые границы слоёв
  for (size_t i = 0; i < layers_; ++i) {
    ext_sizes[i] = sizes_[i];
  }
  ext_sizes[layers_] = points_;
  delete [] sizes_;
  sizes_ = ext_sizes;
  ++layers_;
}

topit::Layers::Layers():
  points_{0}, // кол-во точек
  layers_{0}, // кол-во слоёв
  pts_{nullptr}, // массив точек
  sizes_{nullptr} // границы слоёв
{}

topit::Layers::~Layers()
{
  delete [] pts_;
  delete [] sizes_;
}

topit::Layers::Layers(const Layers& other):
  points_{other.points_},
  layers_{other.layers_},
  pts_{nullptr},
  sizes_{nullptr}
{
  if (points_ != 0) {
    pts_ = new p_t[points_];
    try {
      for (size_t i = 0; i < points_; ++i) {
        pts_[i] = other.pts_[i];
      }
    } catch (...) {
      delete [] pts_;
      pts_ = nullptr;
      throw;
    }
  }
  if (layers_ != 0) {
    sizes_ = new size_t[layers_];
    try {
      for (size_t i =0; i < layers_; ++i) {
        sizes_[i] = other.sizes_[i];
      }
    } catch (...) {
      delete [] pts_;
      delete [] sizes_;
      pts_ = nullptr;
      sizes_ = nullptr;
      throw;
    }
  }
}

topit::Layers& topit::Layers::operator=(const Layers& other)
{
  if (this == &other) {
    return *this;
  }

  Layers tmp(other);
  std::swap(points_, tmp.points_);
  std::swap(pts_, tmp.pts_);
  std::swap(layers_, tmp.layers_);
  std::swap(sizes_, tmp.sizes_);
  return *this;
}
topit::Layers::Layers(Layers&& other) noexcept:
  points_{other.points_},
  pts_{other.pts_},
  layers_{other.layers_},
  sizes_{other.sizes_}
{
  other.points_ = 0;
  other.pts_ = nullptr;
  other.layers_ = 0;
  other.sizes_ = nullptr;
}

topit::Layers& topit::Layers::operator=(Layers&& other) noexcept
{
  if (this == &other) {
    return *this;
  }

  delete [] pts_;
  delete [] sizes_;

  points_ = other.points_;
  pts_ = other.pts_;
  layers_ = other.layers_;
  sizes_ = other.sizes_;

  other.points_ = 0;
  other.pts_ = nullptr;
  other.layers_ = 0;
  other.sizes_ = nullptr;
  return *this;
}

topit::p_t* topit::extend(const p_t* pts, size_t s, p_t fill) {
  p_t* r = new p_t[s+1];
  for (size_t i =0; i < s; ++i) {
    r[i] = pts[i];
  }
  r[s] = fill;
  return r;
}

void topit::extend(p_t** pts, size_t& s, p_t fill) {
  p_t* r = extend(*pts, s, fill);
  delete [] *pts;
  ++s;
  *pts = r;
}

void topit::append(const IDraw* sh, p_t** ppts, size_t& s) {
  extend(ppts, s, sh->begin());
  p_t b = sh->begin();
  while (sh->next(b) != sh->begin()) {
    b = sh->next(b);
    extend(ppts, s, b);
  }
}

void topit::paint(p_t p, char* cnv, f_t fr, char fill) { //координата, которую нужно нарисовать, холст, рамка, символ заполнения
  if (p.x < fr.aa.x || p.x > fr.bb.x || p.y < fr.aa.y || p.y > fr.bb.y) {
    return;
  }
  size_t dx = static_cast<size_t>(p.x - fr.aa.x);
  size_t dy = static_cast<size_t>(fr.bb.y - p.y);
  cnv[dy * cols(fr)+dx] = fill;
}

void topit::flush(std::ostream& os, const char* cnv, f_t fr) {
  for (size_t i = 0; i < rows(fr); ++i) {
    for (size_t j = 0; j < cols(fr); ++j) {
      os << cnv[i * cols(fr) + j];
    }
    os << "\n";
  }
}

char * topit::canvas(f_t fr, char fill) {
  size_t s = rows(fr) * cols(fr);
  char * c = new char[s];
  for (size_t i = 0; i < s; ++i) {
    c[i] = fill;
  }
  return c;
}

topit::f_t topit::frame(const p_t* pts, size_t s) {
  int minx = pts[0].x;
  int miny = pts[0].y;
  int maxx = minx;
  int maxy = miny;
  for (size_t i = 1; i < s; ++i) {
    minx = std::min(minx, pts[i].x); // левый верхний
    miny = std::min(miny, pts[i].y); // левый нижний
    maxx = std::max(maxx, pts[i].x); // правый нижний
    maxy = std::max(maxy, pts[i].y); // правый верхний
  }
  p_t a{minx, miny};
  p_t b{maxx, maxy};
  return f_t{a, b};
}

topit::f_t topit::frame(const Layers& ls) {
  if (ls.points() == 0) throw std::logic_error("No points in Layers");
	p_t* pts = new p_t[ls.points()];
	for (size_t i = 0; i < ls.points(); ++i) {
        pts[i] = ls.point(i);
    }
    f_t fr = topit::frame(pts, ls.points());
    delete[] pts;
    return fr;
}

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

size_t topit::rows(f_t fr) {
  return static_cast<size_t>(fr.bb.y - fr.aa.y + 1);
}

size_t topit::cols(f_t fr) {
  return static_cast<size_t>(fr.bb.x - fr.aa.x + 1);
}

bool topit::operator==(p_t a, p_t b) {
  return a.x == b.x && a.y == b.y;
}

bool topit::operator!=(p_t a, p_t b) {
  return !(a == b);
}

int main() {
  using namespace topit;
  int err = 0;
  IDraw* shp[3] = {};
  Layers layers;
  try {
    shp[0] = new FRect({-10, -4}, 7, 7);
    shp[1] = new FRect({3, 4}, 10, 11);
    shp[2] = new Rect({-3, -2}, 4, 5);
    for (size_t i = 0; i < 3; ++i) {
      layers.append(*(shp[i]));
    }
    f_t fr = layers.frame();
    char * cnv = canvas(fr, '.');
    const char * brush = "#*%";
    for (size_t k = 0; k < layers.layers(); ++k) {
      size_t start = layers.start(k);
      size_t end = layers.end(k);
      for (size_t i = start; i < end; ++i) {
        paint(layers.point(i), cnv, fr, brush[k]);
      }
    }
    flush(std::cout, cnv, fr);
    delete [] cnv;
  } catch (...) {
    std::cerr << "Error\n";
    err = 1;
  }
  delete shp[0];
  delete shp[1];
  delete shp[2];
  return err;
}
