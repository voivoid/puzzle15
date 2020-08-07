#pragma once

namespace puzzle15
{
using coord = int;
using index = size_t;

struct point
{
  coord x;
  coord y;
};

struct pos
{
  index x_idx;
  index y_idx;
};

struct size
{
  coord width;
  coord height;
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

struct rect
{
  point left_top;
  point right_bottom;
};
}  // namespace puzzle15