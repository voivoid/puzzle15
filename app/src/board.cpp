#include "puzzle15/board.h"

#include "puzzle15/assertions.h"

#include <algorithm>
#include <random>
#include <stdexcept>

namespace puzzle15
{

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

const board::opt_cell& board::get_cell( const pos p ) const
{
  return m_cells[ get_cells_vector_idx( p ) ];
}

void board::swap_cells( const pos p1, const pos p2 )
{
  const auto c1_index = get_cells_vector_idx( p1 );
  const auto c2_index = get_cells_vector_idx( p2 );

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
  p15_ensure( free_cells <= m_cells.size() );
  size_t cells_to_fill = m_cells.size() - free_cells;
  size_t n             = 1;
  for ( auto& cell : m_cells )
  {
    cell = cells_to_fill-- ? board::cell{ n++ } : board::opt_cell{};
  }
}

bool board::is_solved( const size_t /*free_cells*/ ) const
{
  return false;
}

bool board::is_free_cell( const pos p )
{
  return !get_cell( p );
}

size_t board::get_cells_vector_idx( const pos p ) const
{
  const auto [ x, y ] = p;
  p15_ensure( x < m_cols );
  p15_ensure( y < m_rows );

  return y * m_cols + x;
}

bool board::move_cell_to_next_free_cell( const pos p )
{
  if ( is_free_cell( p ) )
  {
    return false;
  }

  const auto is_neighbour_free = [ this ]( const auto neighbour_pos ) { return is_free_cell( neighbour_pos ); };

  const auto neighbours      = get_neighbours( p );
  const auto free_neighbours = std::count_if( neighbours.cbegin(), neighbours.cend(), is_neighbour_free );

  if ( free_neighbours == 1 )
  {
    const auto free_neighbour = *std::find_if( neighbours.cbegin(), neighbours.cend(), is_neighbour_free );
    swap_cells( p, free_neighbour );
    return true;
  }

  return false;
}

std::vector<pos> board::get_neighbours( const pos p ) const
{
  std::vector<pos> neighbours;

  if ( p.x_idx > 0 )
  {
    neighbours.push_back( { p.x_idx - 1, p.y_idx } );
  }

  if ( p.x_idx + 1 < m_cols )
  {
    neighbours.push_back( { p.x_idx + 1, p.y_idx } );
  }

  if ( p.y_idx > 0 )
  {
    neighbours.push_back( { p.x_idx, p.y_idx - 1 } );
  }

  if ( p.y_idx + 1 < m_rows )
  {
    neighbours.push_back( { p.x_idx, p.y_idx + 1 } );
  }

  return neighbours;
}

}  // namespace puzzle15