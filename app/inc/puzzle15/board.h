#pragma once

#include "puzzle15/game_events.h"
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
    std::optional<size_t> num;
  };

  board( size_t cols, size_t rows );

  size_t get_cols() const;
  size_t get_rows() const;

  const cell& get_cell( cell_indices c ) const;

  void swap_cells( cell_indices c1, cell_indices c2 );
  std::optional<cell_swapped> try_swap_cell_with_empty_neighbour( cell_indices cell_to_swap );

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

  bool is_free_cell( cell_indices c );

private:
  size_t get_cells_vector_idx( cell_indices c ) const;

private:
  size_t m_cols;
  size_t m_rows;

  std::vector<cell> m_cells;
};
}  // namespace puzzle15