#include "stdafx.h"

#include "puzzle15/platform/game_widget.h"


#include "puzzle15/assertions.h"
#include "puzzle15/board.h"
#include "puzzle15/platform/canvas.h"
#include "puzzle15/platform_ifaces.h"

#include <algorithm>
#include <utility>

namespace
{
namespace timer
{
constexpr UINT_PTR update_game_status = 1;

namespace interval
{
constexpr UINT update_game_status = 1000;
}

}  // namespace timer

}  // namespace

namespace puzzle15
{

game_widget::game_widget( const HINSTANCE instance, const HWND parent )
{
  m_controller = make_controller();

  WNDCLASSEX wcx{};
  wcx.cbSize        = sizeof( wcx );
  wcx.hbrBackground = GetStockBrush( WHITE_BRUSH );
  wcx.hCursor       = ::LoadCursor( nullptr, IDC_ARROW );
  wcx.hInstance     = instance;
  wcx.lpszClassName = L"Puzzle15Control";

  p15_assert( wcx.hbrBackground );
  p15_assert( wcx.hCursor != NULL );

  register_and_create( wcx, WS_CHILD, 0, L"", 0, 0, 0, 0, parent, instance );
  p15_ensure( m_wnd );
}

game_widget::~game_widget() = default;

LRESULT CALLBACK game_widget::wnd_proc( const UINT msg, const WPARAM wParam, const LPARAM lParam )
{
  switch ( msg )
  {
    case WM_PAINT: return on_paint();
    case WM_TIMER: return on_timer( wParam );
    case WM_SIZE: return on_resize( LOWORD( lParam ), HIWORD( lParam ) );
    case WM_LBUTTONDOWN: return on_left_mouse_btn_down( LOWORD( lParam ), HIWORD( lParam ) );
    case WM_LBUTTONUP: return on_left_mouse_btn_up( LOWORD( lParam ), HIWORD( lParam ) );
    case WM_RBUTTONDOWN: return on_right_mouse_btn_up( LOWORD( lParam ), HIWORD( lParam ) );
    case WM_CREATE: return on_create();
  }

  return ::DefWindowProc( m_wnd, msg, wParam, lParam );
}

hdc_canvas game_widget::make_canvas( const PAINTSTRUCT& /*ps*/, const HDC dc, const size_t cell_side ) const
{
  hdc_canvas canvas( dc, get_client_size(), cell_side );
  return canvas;
}

LRESULT game_widget::on_create()
{
  bool success = ::SetTimer( m_wnd, timer::update_game_status, timer::interval::update_game_status, nullptr ) != 0;
  p15_assert( success );
  return 0;
}

LRESULT game_widget::on_paint()
{
  PAINTSTRUCT ps;
  HDC dc = ::BeginPaint( m_wnd, &ps );
  p15_assert( dc );

  const auto old_font = ::SelectObject( dc, m_font.get() );
  p15_assert( old_font );

  auto canvas = make_canvas( ps, dc, calc_current_cell_side() );
  m_controller->draw_board( canvas );

  bool object_selected = ::SelectObject( dc, old_font ) != NULL;
  p15_assert( object_selected );

  bool paint_finished = ::EndPaint( m_wnd, &ps );
  p15_assert( paint_finished );

  return 0;
}

LRESULT game_widget::on_resize( const WORD /*width*/, const WORD /*height*/ )
{
  const auto cell_font_height = static_cast<int>( calc_current_cell_side() / 3 );
  m_font.reset( ::CreateFont( cell_font_height,
                              0,
                              0,
                              0,
                              FW_DONTCARE,
                              0,
                              0,
                              0,
                              DEFAULT_CHARSET,
                              OUT_OUTLINE_PRECIS,
                              CLIP_DEFAULT_PRECIS,
                              CLEARTYPE_QUALITY,
                              VARIABLE_PITCH,
                              NULL ) );

  p15_assert( m_font );

  return 0;
}

LRESULT game_widget::on_left_mouse_btn_down( const DWORD /*mouseX*/, const DWORD /*mouseY*/ )
{
  return 0;
}

LRESULT game_widget::on_left_mouse_btn_up( const DWORD /*mouseX*/, const DWORD /*mouseY*/ )
{
  return 0;
}

LRESULT game_widget::on_right_mouse_btn_up( const DWORD mouseX, const DWORD mouseY )
{
  const auto [ x_cell_idx, y_cell_idx ] = get_cell_index_by_coords( mouseX, mouseY );
  auto cell_moved                       = m_controller->handle_right_mouse_click( x_cell_idx, y_cell_idx );
  if ( cell_moved )
  {
    move_cell( cell_moved->from, cell_moved->to );
  }

  return 0;
}

LRESULT game_widget::on_timer( const WPARAM timer_id )
{
  if ( timer_id == timer::update_game_status )
  {
    update_game_stats();
  }

  return 0;
}

rect_size game_widget::calc_size_to_fit( const rect_size available_size ) const
{
  const auto board_dims = m_controller->get_board_dims();
  const auto cell_side  = calc_max_cell_side_to_fit( available_size );

  return { cell_side * board_dims.cols, cell_side * board_dims.rows };
}

void game_widget::reset_game()
{
  m_controller->reset_game();
  redraw();
  update_game_stats();
}

void game_widget::redraw()
{
  bool success = ::InvalidateRect( m_wnd, nullptr, TRUE );
  p15_assert( success );
}

void game_widget::redraw_cell( const cell_indices idx )
{
  const RECT cell_rect = get_cell_rect_by_index( idx );
  bool success         = ::InvalidateRect( m_wnd, &cell_rect, TRUE );
  p15_assert( success );
}

void game_widget::move_cell( const cell_indices from, const cell_indices to )
{
  redraw_cell( from );
  redraw_cell( to );

  update_game_stats();
}

size_t game_widget::calc_max_cell_side_to_fit( const rect_size available_size ) const
{
  const auto board_dims      = m_controller->get_board_dims();
  const auto min_width_size  = available_size.width / board_dims.cols;
  const auto min_height_size = available_size.height / board_dims.rows;

  return std::min( min_width_size, min_height_size );
}

size_t game_widget::calc_current_cell_side() const
{
  return calc_max_cell_side_to_fit( get_client_size() );
}

cell_indices game_widget::get_cell_index_by_coords( const DWORD mouseX, const DWORD mouseY ) const
{
  p15_assert( get_client_size().width % m_controller->get_board_dims().cols == 0 );
  p15_assert( get_client_size().height % m_controller->get_board_dims().rows == 0 );
  p15_assert( mouseX >= 0 );
  p15_assert( mouseY >= 0 );

  const auto cell_side = calc_current_cell_side();
  return { static_cast<size_t>( mouseX ) / cell_side, static_cast<size_t>( mouseY ) / cell_side };
}

RECT game_widget::get_cell_rect_by_index( cell_indices idx ) const
{
  const auto cell_side = static_cast<coord>( calc_current_cell_side() );
  auto left            = static_cast<coord>( idx.x_idx * cell_side );
  auto top             = static_cast<coord>( idx.y_idx * cell_side );

  RECT rect;
  rect.left   = left;
  rect.top    = top;
  rect.right  = left + cell_side;
  rect.bottom = top + cell_side;

  return rect;
}

rect_size game_widget::calc_border_to_fit( rect_size available_size ) const
{
  const auto game_widget_size = calc_size_to_fit( available_size );

  p15_assert( game_widget_size.width <= available_size.width );
  p15_assert( game_widget_size.height <= available_size.height );

  return { ( available_size.width - game_widget_size.width ) / 2, ( available_size.height - game_widget_size.height ) / 2 };
}

void game_widget::set_status_updater( const status_updater& updater )
{
  m_status_updater = updater;
}

void game_widget::update_game_stats()
{
  m_status_updater( m_controller->get_game_stats() );
}

}  // namespace puzzle15
