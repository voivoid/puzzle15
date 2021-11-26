#pragma once

#include "puzzle15/board.h"
#include "puzzle15/game_events.h"
#include "puzzle15/platform_ifaces.h"

#include <optional>

namespace puzzle15
{

class game : public controller
{
public:
  game();

private:
  virtual dims get_board_dims() const override;
  virtual void draw_board( canvas& ) override;

  virtual void reset_game() override;

  virtual game_stats get_game_stats() const override;

  virtual std::optional<cell_swapped> handle_right_mouse_click( size_t x_cell_idx, size_t y_cell_idx ) override;

private:
  board m_board;
  size_t m_moves;
  std::optional<std::chrono::steady_clock::time_point> m_start_time;
};
}  // namespace puzzle15