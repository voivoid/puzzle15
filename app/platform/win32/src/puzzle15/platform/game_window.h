#pragma once

#include "puzzle15/platform/window.h"

namespace puzzle15
{

class game_control;
class game_window final : public base_window
{
public:
  game_window( HINSTANCE, int cmd_show );
  ~game_window();

private:
  // window handlers:
  virtual LRESULT CALLBACK wnd_proc( UINT msg, WPARAM wParam, LPARAM lParam ) override;

  LRESULT on_close();
  LRESULT on_destroy();
  LRESULT on_size();

  LRESULT handle_command( WPARAM wParam, LPARAM lParam );
  LRESULT handle_menu_command( int menu_id );

  void quit();
  void update_game_control_pos();

  size calc_border() const;

private:
  std::unique_ptr<game_control> m_game_control;
};

}  // namespace puzzle15