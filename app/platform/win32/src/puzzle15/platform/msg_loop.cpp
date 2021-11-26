#include "stdafx.h"

#include "puzzle15/platform/msg_loop.h"

#include "puzzle15/assertions.h"

namespace puzzle15
{
int run_message_loop()
{
  MSG msg{};
  BOOL ret{};

  while ( ret = ::GetMessage( &msg, nullptr, 0, 0 ), ret > 0 )
  {
    ::TranslateMessage( &msg );
    ::DispatchMessage( &msg );
  }
  p15_assert( ret != -1 );

  return static_cast<int>( msg.wParam );
}
}  // namespace puzzle15