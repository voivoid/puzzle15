#include "stdafx.h"

#include "puzzle15/assertions.h"
#include "puzzle15/platform/error.h"
#include "puzzle15/platform/game_window.h"

#include <stdexcept>

namespace
{

int run_game_loop( const HINSTANCE instance, const int cmd_show )
{
  auto window = puzzle15::game_window( instance, cmd_show );

  MSG msg{};
  BOOL ret{};
  while ( ret = ::GetMessage( &msg, nullptr, 0, 0 ), ret > 0 )
  {
    ::TranslateMessage( &msg );
    ::DispatchMessage( &msg );
  }
  p15_ensure( ret != -1 );

  return static_cast<int>( msg.wParam );
}
}  // namespace

int WINAPI wWinMain( HINSTANCE instance, HINSTANCE, PWSTR, int cmd_show )
{
  try
  {
    return run_game_loop( instance, cmd_show );
  }
  catch ( const std::exception& ex )
  {
    puzzle15::show_exception_message( ex.what() );
  }
  catch ( ... )
  {
    puzzle15::show_exception_message( "unknown exception" );
  }

  return -1;
}