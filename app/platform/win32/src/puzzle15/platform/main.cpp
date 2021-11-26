#include "stdafx.h"

#include "puzzle15/platform/error.h"
#include "puzzle15/platform/game_window.h"
#include "puzzle15/platform/msg_loop.h"

#include <crtdbg.h>
#include <stdexcept>

namespace
{
int run_game( HINSTANCE instance, int cmd_show )
{
  const INITCOMMONCONTROLSEX commctrls{ .dwSize = sizeof( commctrls ), .dwICC = ICC_BAR_CLASSES | ICC_STANDARD_CLASSES };
  ::InitCommonControlsEx( &commctrls );

  auto window = puzzle15::game_window( instance, cmd_show );
  return puzzle15::run_message_loop();
}
}  // namespace

int WINAPI wWinMain( HINSTANCE instance, HINSTANCE, PWSTR, int cmd_show )
{
  try
  {
    int retval = run_game( instance, cmd_show );

#ifndef NDEBUG
    auto has_leaks = _CrtDumpMemoryLeaks();
    p15_assert( !has_leaks );
#endif

    return retval;
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