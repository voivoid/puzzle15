#pragma once

#include "puzzle15/game_events.h"
#include "puzzle15/game_stats.h"
#include "puzzle15/geo.h"

#include <chrono>
#include <memory>
#include <optional>
#include <string>

namespace puzzle15
{
struct canvas
{
  virtual ~canvas() = default;

  virtual void draw_rect( const rectangle& r )                      = 0;
  virtual void draw_text( const rectangle& r, const std::wstring& ) = 0;

  virtual size_t width() const     = 0;
  virtual size_t height() const    = 0;
  virtual size_t cell_side() const = 0;
};

struct controller
{
  virtual ~controller() = default;

  virtual dims get_board_dims() const = 0;
  virtual void draw_board( canvas& )  = 0;

  virtual void reset_game() = 0;

  virtual game_stats get_game_stats() const = 0;

  virtual std::optional<cell_swapped> handle_right_mouse_click( size_t x_cell_idx, size_t y_cell_idx ) = 0;
};

std::unique_ptr<controller> make_controller();
}  // namespace puzzle15