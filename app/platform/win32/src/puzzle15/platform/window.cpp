#include "stdafx.h"

#include "puzzle15/platform/window.h"

#include "puzzle15/assertions.h"

namespace puzzle15
{

base_window::base_window() : m_wnd( nullptr )
{
}

base_window::~base_window() = default;


void base_window::register_and_create( WNDCLASSEX& wcx,
                                       const DWORD style,
                                       const DWORD ex_style,
                                       const LPCWSTR window_name,
                                       const int x,
                                       const int y,
                                       const int width,
                                       const int height,
                                       const HWND parent,
                                       const HINSTANCE instance )
{
  p15_assert( !m_wnd );

  const auto [ init_wnd_proc, base_wnd_proc ] = m_thunk.generate_callbacks( this, &m_wnd );
  p15_assert( init_wnd_proc );
  p15_assert( base_wnd_proc );

  wcx.lpfnWndProc = init_wnd_proc;

  ATOM wndClass = ::RegisterClassEx( &wcx );
  p15_assert( wndClass );

  HWND wnd = ::CreateWindowEx( ex_style, wcx.lpszClassName, window_name, style, x, y, width, height, parent, nullptr, instance, nullptr );
  p15_assert( wnd );
  p15_assert( wnd == m_wnd );

  p15_ensure( SubclassWindow( wnd, base_wnd_proc ) );
}

rect_size base_window::get_client_size() const
{
  RECT client_rect{};
  p15_ensure( ::GetClientRect(m_wnd, &client_rect) );

  const auto width  = client_rect.right - client_rect.left;
  const auto height = client_rect.bottom - client_rect.top;
  p15_assert( width >= 0 );
  p15_assert( height >= 0 );

  return { static_cast<size_t>( width ), static_cast<size_t>( height ) };
}

void base_window::show( const int cmd_show )
{
  p15_ensure( ::ShowWindow( m_wnd, cmd_show ) == 0 );
  p15_ensure( ::UpdateWindow( m_wnd ) );
}

void base_window::move( const point p, const rect_size s )
{
  p15_ensure( ::MoveWindow( m_wnd, p.x, p.y, static_cast<int>( s.width ), static_cast<int>( s.height ), TRUE ) );
}

void base_window::destroy()
{
  p15_ensure( ::DestroyWindow( m_wnd ) );
}

}  // namespace puzzle15