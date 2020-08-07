#pragma once

#include "puzzle15/geo.h"

#include <optional>
#include <string>
#include <vector>

namespace puzzle15
{
class board
{
public:
  struct cell
  {
    size_t num;
  };

  board( size_t cols, size_t rows );

  size_t get_cols() const;
  size_t get_rows() const;

  using opt_cell = std::optional<cell>;
  const opt_cell& get_cell( pos p ) const;

  void swap_cells( pos p1, pos p2 );
  bool move_cell_to_next_free_cell( pos p );

  void clear();
  void random_fill( size_t free_cells = 1 );
  void serial_fill( size_t free_cells = 1 );

  bool is_solved( size_t free_cells = 1 ) const;

  template <typename F>
  void for_each_cell( const F& f )
  {
    for ( size_t x = 0; x < m_cols; ++x )
    {
      for ( size_t y = 0; y < m_rows; ++y )
      {
        f( x, y, get_cell( { x, y } ) );
      }
    }
  }

  bool is_free_cell( pos p );

private:
  size_t get_cells_vector_idx( pos p ) const;

  std::vector<pos> get_neighbours( pos p ) const;

private:
  size_t m_cols;
  size_t m_rows;

  std::vector<opt_cell> m_cells;
};
}  // namespace puzzle15