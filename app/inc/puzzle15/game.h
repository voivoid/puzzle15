#pragma once

#include "puzzle15/board.h"
#include "puzzle15/platform_iface.h"

namespace puzzle15
{
class game : public controller
{
public:
  game();

private:
  virtual dims get_board_dims() const override;
  virtual void reset_game() override;

  virtual void draw_frame( const coord cell_size, canvas& ) override;

  virtual bool handle_right_mouse_click( size_t x_cell_idx, size_t y_cell_idx ) override;

private:
  board m_board;
};
}  // namespace puzzle15