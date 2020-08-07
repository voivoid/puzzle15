#pragma once

#include "puzzle15/geo.h"

#include <memory>
#include <string>

namespace puzzle15
{
struct canvas
{
  virtual ~canvas() = default;

  virtual void draw_rect( const rect& r )               = 0;
  virtual void draw_text( rect r, const std::wstring& ) = 0;

  virtual coord width() const  = 0;
  virtual coord height() const = 0;
};

class board;
struct controller
{
  virtual ~controller() = default;

  virtual dims get_board_dims() const = 0;
  virtual void reset_game()           = 0;

  virtual void draw_frame( coord cell_size, canvas& ) = 0;

  virtual bool handle_right_mouse_click( size_t x_cell_idx, size_t y_cell_idx ) = 0;
};

std::unique_ptr<controller> make_controller();
}  // namespace puzzle15