#pragma once

#include <windows.h>

#include "puzzle15/game_stats.h"

namespace puzzle15
{

class game_status_bar
{
public:
  game_status_bar( HINSTANCE instance, HWND parent );

  void update_size();
  void update_content( const game_stats& data );

  int get_height() const;

private:
  HWND m_wnd;
};
}  // namespace puzzle15