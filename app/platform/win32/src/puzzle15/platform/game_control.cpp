#include "stdafx.h"

#include "puzzle15/assertions.h"
#include "puzzle15/board.h"
#include "puzzle15/platform/canvas.h"
#include "puzzle15/platform/game_control.h"
#include "puzzle15/platform_iface.h"

#include <algorithm>
#include <utility>

namespace puzzle15
{

game_control::game_control( const HINSTANCE instance, const HWND parent )
{
  m_controller = make_controller();

  WNDCLASSEX wcx{};
  wcx.cbSize        = sizeof( wcx );
  wcx.hbrBackground = GetStockBrush( WHITE_BRUSH );
  wcx.hCursor       = ::LoadCursor( nullptr, IDC_ARROW );
  wcx.hInstance     = instance;
  wcx.lpszClassName = L"Puzzle15Control";

  p15_ensure( wcx.hbrBackground );
  p15_ensure( wcx.hCursor != NULL );

  register_and_create( wcx, WS_CHILD, 0, L"", 0, 0, 0, 0, parent, instance );
  p15_ensure( m_wnd );
}

game_control::~game_control() = default;

LRESULT CALLBACK game_control::wnd_proc( const UINT msg, const WPARAM wParam, const LPARAM lParam )
{
  switch ( msg )
  {
    case WM_PAINT: return on_paint();
    case WM_SIZE: return on_size( LOWORD( lParam ), HIWORD( lParam ) );
    case WM_LBUTTONDOWN: return on_left_mouse_btn_down( LOWORD( lParam ), HIWORD( lParam ) );
    case WM_LBUTTONUP: return on_left_mouse_btn_up( LOWORD( lParam ), HIWORD( lParam ) );
    case WM_RBUTTONDOWN: return on_right_mouse_btn_up( LOWORD( lParam ), HIWORD( lParam ) );
  }

  return ::DefWindowProc( m_wnd, msg, wParam, lParam );
}

hdc_canvas game_control::make_canvas( const PAINTSTRUCT& /*ps*/, HDC dc ) const
{
  hdc_canvas canvas( dc, get_client_size() );
  return canvas;
}

LRESULT game_control::on_paint()
{
  PAINTSTRUCT ps;
  HDC dc = ::BeginPaint( m_wnd, &ps );
  p15_ensure( dc );

  const auto old_font = ::SelectObject( dc, m_font.get() );
  p15_ensure( old_font );

  auto canvas = make_canvas( ps, dc );
  m_controller->draw_frame( calc_current_cell_side(), canvas );

  p15_ensure( ::SelectObject( dc, old_font ) );

  p15_ensure( ::EndPaint( m_wnd, &ps ) );

  return 0;
}

LRESULT game_control::on_size( const WORD /*width*/, const WORD /*height*/ )
{
  const auto cell = calc_current_cell_side();
  m_font.reset( ::CreateFont( -( cell / 3 ), 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, NULL ) );

  p15_ensure( m_font );

  return 0;
}

LRESULT game_control::on_left_mouse_btn_down( const DWORD /*x*/, const DWORD /*y*/ )
{
  return 0;
}

LRESULT game_control::on_left_mouse_btn_up( const DWORD /*x*/, const DWORD /*y*/ )
{
  return 0;
}

LRESULT game_control::on_right_mouse_btn_up( const DWORD x, const DWORD y )
{
  const auto [ x_cell_idx, y_cell_idx ] = calc_cell_index( x, y );
  bool handled                          = m_controller->handle_right_mouse_click( x_cell_idx, y_cell_idx );
  if ( handled )
  {
    redraw();
  }

  return 0;
}

size game_control::calc_size( const size window_size ) const
{
  const auto board_dims = m_controller->get_board_dims();
  const auto cell_side  = calc_cell_side( window_size );

  return { cell_side * static_cast<coord>( board_dims.cols ), cell_side * static_cast<coord>( board_dims.rows ) };
}

void game_control::reset_game()
{
  m_controller->reset_game();
  redraw();
}

void game_control::redraw()
{
  p15_ensure( ::InvalidateRect( m_wnd, nullptr, TRUE ) );
}

coord game_control::calc_cell_side( const size available_size ) const
{
  const auto board_dims      = m_controller->get_board_dims();
  const auto min_width_size  = available_size.width / board_dims.cols;
  const auto min_height_size = available_size.height / board_dims.rows;

  return std::min( static_cast<coord>( min_width_size ), static_cast<coord>( min_height_size ) );
}

coord game_control::calc_current_cell_side() const
{
  return calc_cell_side( get_client_size() );
}

pos game_control::calc_cell_index( const DWORD x, const DWORD y )
{
  assert( get_client_size().width % m_controller->get_board_dims().cols == 0 );
  assert( get_client_size().height % m_controller->get_board_dims().rows == 0 );

  const coord cell_side = calc_current_cell_side();
  return { size_t( x / cell_side ), size_t( y / cell_side ) };
}

}  // namespace puzzle15
