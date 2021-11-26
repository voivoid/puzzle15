#include "catch2/catch_test_macros.hpp"

#include "puzzle15/board.h"

using namespace puzzle15;

namespace
{
struct generic_board_fixture
{
  generic_board_fixture() : m_width( 5 ), m_height( 5 ), m_board{ m_width, m_height }
  {
    m_board.random_fill();
  }

  const size_t m_width;
  const size_t m_height;
  board m_board;
};
}  // namespace

TEST_CASE( "Board returns it's dimensions", "[board]" )
{
  board b1{ 1, 1 };
  CHECK( b1.get_cols() == 1 );
  CHECK( b1.get_rows() == 1 );

  board b2{ 3, 4 };
  CHECK( b2.get_cols() == 3 );
  CHECK( b2.get_rows() == 4 );

  board b3{ 4, 3 };
  CHECK( b3.get_cols() == 4 );
  CHECK( b3.get_rows() == 3 );

  CHECK_THROWS( board{ 0, 1 } );
  CHECK_THROWS( board{ 1, 0 } );
}

TEST_CASE( "New board is empty", "[board]" )
{
  size_t width  = 3;
  size_t height = 5;

  board b{ width, height };
  for ( size_t x = 0; x < width; ++x )
  {
    for ( size_t y = 0; y < height; ++y )
    {
      CHECK( !b.get_cell( { x, y } ).num );
    }
  }
}

TEST_CASE_METHOD( generic_board_fixture, "Board should be copyable", "[board]" )
{
}