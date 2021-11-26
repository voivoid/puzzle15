#include "puzzle15/board.h"

#include "puzzle15/assertions.h"

#include <algorithm>
#include <random>
#include <stdexcept>

#include "boost/container/small_vector.hpp"

namespace puzzle15
{

namespace
{
using neighbours = boost::container::small_vector<cell_indices, 4>;
neighbours get_neighbours( const cell_indices c, const size_t cols, const size_t rows )
{
  neighbours neighbours;

  if ( c.x_idx > 0 )
  {
    neighbours.push_back( { c.x_idx - 1, c.y_idx } );
  }

  if ( c.x_idx + 1 < cols )
  {
    neighbours.push_back( { c.x_idx + 1, c.y_idx } );
  }

  if ( c.y_idx > 0 )
  {
    neighbours.push_back( { c.x_idx, c.y_idx - 1 } );
  }

  if ( c.y_idx + 1 < rows )
  {
    neighbours.push_back( { c.x_idx, c.y_idx + 1 } );
  }

  return neighbours;
}
}  // namespace

board::board( const size_t cols, const size_t rows ) : m_cols( cols ), m_rows( rows )
{
  if ( cols == 0 || rows == 0 )
  {
    throw std::invalid_argument( "board has invalid size" );
  }

  m_cells.resize( cols * rows );
}

size_t board::get_cols() const
{
  return m_cols;
}

size_t board::get_rows() const
{
  return m_rows;
}

const board::cell& board::get_cell( const cell_indices c ) const
{
  auto cell_idx = get_cells_vector_idx( c );
  return m_cells[ cell_idx ];
}

void board::swap_cells( const cell_indices c1, const cell_indices c2 )
{
  const auto c1_index = get_cells_vector_idx( c1 );
  const auto c2_index = get_cells_vector_idx( c2 );

  std::swap( m_cells[ c1_index ], m_cells[ c2_index ] );
}

void board::clear()
{
  for ( auto& cell : m_cells )
  {
    cell = {};
  }
}

void board::random_fill( const size_t free_cells )
{
  serial_fill( free_cells );

  std::random_device rd;
  std::mt19937 g( rd() );
  std::shuffle( m_cells.begin(), m_cells.end(), g );
}

void board::serial_fill( const size_t free_cells )
{
  p15_assert( free_cells <= m_cells.size() );
  size_t cells_to_fill = m_cells.size() - free_cells;
  size_t n             = 1;
  for ( auto& cell : m_cells )
  {
    cell = cells_to_fill-- ? board::cell{ { n++ } } : board::cell{ {} };
  }
}

bool board::is_solved( const size_t /*free_cells*/ ) const
{
  return false;  // TODO
}

bool board::is_free_cell( const cell_indices c )
{
  return !get_cell( c ).num;
}

size_t board::get_cells_vector_idx( const cell_indices c ) const
{
  const auto [ x, y ] = c;
  p15_assert( x < m_cols );
  p15_assert( y < m_rows );

  return y * m_cols + x;
}

std::optional<cell_swapped> board::try_swap_cell_with_empty_neighbour( const cell_indices cell_to_swap )
{
  if ( is_free_cell( cell_to_swap ) )
  {
    return {};
  }

  const auto is_neighbour_free = [ this ]( const auto neighbour_pos ) { return is_free_cell( neighbour_pos ); };

  const auto neighbours      = get_neighbours( cell_to_swap, m_cols, m_rows );
  const auto free_neighbours = std::count_if( neighbours.cbegin(), neighbours.cend(), is_neighbour_free );

  if ( free_neighbours == 1 )
  {
    const auto free_neighbour = *std::find_if( neighbours.cbegin(), neighbours.cend(), is_neighbour_free );
    swap_cells( cell_to_swap, free_neighbour );
    return { cell_swapped{ cell_to_swap, free_neighbour } };
  }

  return {};
}

}  // namespace puzzle15