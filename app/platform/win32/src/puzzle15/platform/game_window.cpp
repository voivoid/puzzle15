#include "stdafx.h"

#include "puzzle15/platform/game_window.h"

#include "puzzle15/assertions.h"
#include "puzzle15/platform/game_status_bar.h"
#include "puzzle15/platform/game_widget.h"
#include "puzzle15/platform/res/resource.h"

#include <functional>

namespace puzzle15
{

game_window::game_window( const HINSTANCE instance, const int cmd_show )
{
  WNDCLASSEX wcx{};
  wcx.cbSize        = sizeof( wcx );
  wcx.hbrBackground = GetStockBrush( WHITE_BRUSH );
  wcx.hCursor       = ::LoadCursor( nullptr, IDC_ARROW );
  wcx.hIcon         = ::LoadIcon( nullptr, IDI_APPLICATION );
  wcx.hIconSm       = wcx.hIcon;
  wcx.hInstance     = instance;
  wcx.lpszClassName = L"Puzzle15Window";
  wcx.lpszMenuName  = MAKEINTRESOURCE( IDR_GAME_MENU );
  wcx.style         = CS_HREDRAW | CS_VREDRAW;

  p15_assert( wcx.hbrBackground );
  p15_assert( wcx.hCursor );
  p15_assert( wcx.hIcon );

  register_and_create(
      wcx, WS_OVERLAPPEDWINDOW, WS_EX_OVERLAPPEDWINDOW, L"Puzzle 15", CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, nullptr, instance );
  p15_ensure( m_wnd );

  m_status_bar.reset( new game_status_bar( instance, m_wnd ) );
  m_game_widget.reset( new game_widget( instance, m_wnd ) );

  m_game_widget->set_status_updater( std::bind( &game_status_bar::update_content, m_status_bar.get(), std::placeholders::_1 ) );

  update_game_widget_pos();
  m_game_widget->show( SW_SHOW );

  show( cmd_show );
}

game_window::~game_window() = default;

LRESULT CALLBACK game_window::wnd_proc( const UINT msg, const WPARAM wParam, const LPARAM lParam )
{
  switch ( msg )
  {
    case WM_CLOSE: return on_close();
    case WM_DESTROY: return on_destroy();
    case WM_SIZE: return on_size();
    case WM_COMMAND: return handle_command( wParam, lParam );
  }

  return ::DefWindowProc( m_wnd, msg, wParam, lParam );
}

LRESULT game_window::on_close()
{
  destroy();
  return 0;
}

LRESULT game_window::on_destroy()
{
  quit();
  return 0;
}

LRESULT game_window::on_size()
{
  update_game_widget_pos();
  m_status_bar->update_size();
  return 0;
}


LRESULT game_window::handle_command( const WPARAM wParam, const LPARAM lParam )
{
  if ( HIWORD( wParam ) == 0 && lParam == 0 )
  {
    return handle_menu_command( LOWORD( wParam ) );
  }

  p15_ensure( false );
  return 0;
}

LRESULT game_window::handle_menu_command( const int menu_id )
{
  switch ( menu_id )
  {
    case ID_GAME_NEW: m_game_widget->reset_game(); return 0;
    case ID_GAME_EXIT: quit(); return 0;
  }

  p15_ensure( false );
  return 0;
}

void game_window::quit()
{
  ::PostQuitMessage( 0 );
}

void game_window::update_game_widget_pos()
{
  const auto client_size = get_client_size().crop_height( m_status_bar->get_height() );

  const auto game_widget_border = m_game_widget->calc_border_to_fit( client_size );

  auto game_widget_width  = client_size.width - game_widget_border.width * 2;
  auto game_widget_height = client_size.height - game_widget_border.height * 2;

  game_widget_width -= game_widget_width % 2;
  game_widget_height -= game_widget_height % 2;

  m_game_widget->move( { static_cast<int>( game_widget_border.width ), static_cast<int>( game_widget_border.height ) },
                       { game_widget_width, game_widget_height } );
}

}  // namespace puzzle15