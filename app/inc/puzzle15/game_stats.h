#pragma once

#include <chrono>

namespace puzzle15
{

struct game_stats
{
  size_t moves;
  std::chrono::seconds time_since_start;
};

}  // namespace puzzle15