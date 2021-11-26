#pragma once

#include "puzzle15/assertions.h"

namespace puzzle15
{
using coord = int;
using index = size_t;

struct point
{
  coord x;
  coord y;
};

inline point operator+( point p, coord c )
{
  return { p.x + c, p.y + c };
}

struct cell_indices
{
  index x_idx;
  index y_idx;
};

struct rect_size
{
  size_t width;
  size_t height;

  rect_size crop_height( size_t s ) const
  {
    p15_ensure( height >= s );
    return { width, height - s };
  }
};

struct dims
{
  index cols;
  index rows;
};

struct line
{
  point p1;
  point p2;
};

struct rectangle
{
  point left_top;
  point right_bottom;
};
}  // namespace puzzle15