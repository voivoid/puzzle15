#include "puzzle15/game.h"

#include "puzzle15/assertions.h"

namespace puzzle15
{

namespace
{
void draw_cell( canvas& canvas, const board::cell& cell, const rectangle& cell_rect )
{
  p15_assert( cell.num );
  canvas.draw_rect( cell_rect );
  canvas.draw_text( cell_rect, std::to_wstring( *cell.num ) );
}

}  // namespace

game::game() : m_board( 4, 4 ), m_moves( 0 )
{
  m_board.serial_fill();
}

dims game::get_board_dims() const
{
  return { m_board.get_cols(), m_board.get_rows() };
}

void game::reset_game()
{
  m_moves      = 0;
  m_start_time = std::chrono::steady_clock::now();
  m_board.random_fill();
}

void game::draw_board( canvas& canvas )
{
  constexpr coord cell_border = 5;

  const coord cell_side      = static_cast<coord>( canvas.cell_side() );
  const coord cell_rect_side = cell_side - ( cell_border * 2 );
  p15_assert( cell_rect_side > 0 );

  m_board.for_each_cell(
      [ &canvas, cell_side, cell_rect_side ]( const size_t x_idx, const size_t y_idx, const board::cell& cell )
      {
        if ( cell.num )
        {
          const int rect_left  = cell_border + static_cast<coord>( x_idx * cell_side );
          const int rect_top   = cell_border + static_cast<coord>( y_idx * cell_side );
          const point left_top = { rect_left, rect_top };

          const rectangle cell_rect = { left_top, left_top + cell_rect_side };
          draw_cell( canvas, cell, cell_rect );
        }
      } );
}

std::optional<cell_swapped> game::handle_right_mouse_click( size_t x_cell_idx, size_t y_cell_idx )
{
  auto cell_swapped = m_board.try_swap_cell_with_empty_neighbour( { x_cell_idx, y_cell_idx } );
  if ( cell_swapped )
  {
    ++m_moves;
  }
  return cell_swapped;
}

std::unique_ptr<controller> make_controller()
{
  return std::unique_ptr<controller>{ new game };
}

game_stats game::get_game_stats() const
{
  game_stats stats{};
  stats.moves = m_moves;

  if ( m_start_time )
  {
    const auto duration    = std::chrono::steady_clock::now() - *m_start_time;
    stats.time_since_start = std::chrono::duration_cast<std::chrono::seconds>( duration );
  }

  return stats;
}

}  // namespace puzzle15