#include "stdafx.h"

#include "puzzle15/platform/game_status_bar.h"

#include "puzzle15/assertions.h"

#include <string>

namespace puzzle15
{
game_status_bar::game_status_bar( const HINSTANCE instance, const HWND parent )
{
  m_wnd = ::CreateWindow( STATUSCLASSNAME,
                          L"",
                          WS_CHILD | WS_VISIBLE,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          parent,
                          nullptr,
                          instance,
                          nullptr );
  p15_ensure( m_wnd );

  int arr[] = { 100, -1 };
  ::SendMessage( m_wnd, SB_SETPARTS, 2, std::bit_cast<LPARAM>( &arr ) );
}

void game_status_bar::update_size()
{
  bool success = ::SendMessage( m_wnd, WM_SIZE, 0, 0 ) == 0;
  p15_assert( success );
}

void game_status_bar::update_content( const game_stats& stats )
{
  std::wstring moves = L"moves: " + std::to_wstring( stats.moves );
  bool success       = ::SendMessage( m_wnd, SB_SETTEXT, 0, std::bit_cast<LPARAM>( moves.c_str() ) );
  p15_assert( success );

  std::wstring timer = L"timer: " + std::to_wstring( stats.time_since_start.count() );
  success            = ::SendMessage( m_wnd, SB_SETTEXT, 1, std::bit_cast<LPARAM>( timer.c_str() ) );
  p15_assert( success );
}

int game_status_bar::get_height() const
{
  RECT rect;
  bool success = ::SendMessage( m_wnd, SB_GETRECT, 0, std::bit_cast<WPARAM>( &rect ) );
  p15_assert( success );

  return rect.bottom - rect.top;
}
}  // namespace puzzle15