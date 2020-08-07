#include "puzzle15/game.h"

#include "puzzle15/assertions.h"

namespace puzzle15
{

namespace
{
void draw_cell( canvas& canvas, const board::cell& cell, const point left_top, const coord size )
{
  canvas.draw_rect( { left_top.x, left_top.y, left_top.x + size, left_top.y + size } );
  canvas.draw_text( { left_top, { left_top.x + size, left_top.y + size } }, std::to_wstring( cell.num ) );
}

}  // namespace

game::game() : m_board( 4, 4 )
{
  m_board.serial_fill();
}

dims game::get_board_dims() const
{
  return { m_board.get_cols(), m_board.get_rows() };
}

void game::reset_game()
{
  m_board.random_fill();
}

void game::draw_frame( const coord cell_size, canvas& canvas )
{
  const coord border = 5;
  m_board.for_each_cell( [ &canvas, border, cell_size ]( const size_t x_idx, const size_t y_idx, const board::opt_cell& cell ) {
    if ( cell )
    {
      const int x = border + static_cast<int>( x_idx * cell_size );
      const int y = border + static_cast<int>( y_idx * cell_size );
      draw_cell( canvas, *cell, { x, y }, ( cell_size - border * 2 ) );
    }
  } );
}

bool game::handle_right_mouse_click( size_t x_cell_idx, size_t y_cell_idx )
{
  return m_board.move_cell_to_next_free_cell( { x_cell_idx, y_cell_idx } );
}

std::unique_ptr<controller> make_controller()
{
  return std::unique_ptr<controller>{ new game };
}

}  // namespace puzzle15