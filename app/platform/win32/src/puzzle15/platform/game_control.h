#pragma once

#include "puzzle15/platform/handles.h"
#include "puzzle15/platform/window.h"

#include <memory>

namespace puzzle15
{
struct controller;
struct hdc_canvas;
class game_control : public base_window
{
public:
  game_control( HINSTANCE instance, HWND parent );
  ~game_control();

  void reset_game();
  size calc_size( size window_size ) const;

private:
  virtual LRESULT CALLBACK wnd_proc( UINT msg, WPARAM wParam, LPARAM lParam ) override;

  LRESULT on_paint();

  LRESULT on_size( WORD width, WORD height );
  LRESULT on_left_mouse_btn_down( DWORD x, DWORD y );
  LRESULT on_left_mouse_btn_up( DWORD x, DWORD y );
  LRESULT on_right_mouse_btn_up( DWORD x, DWORD y );

  hdc_canvas make_canvas( const PAINTSTRUCT& ps, HDC dc ) const;

  coord calc_cell_side( const size available_size ) const;
  coord calc_current_cell_side() const;
  pos calc_cell_index( const DWORD x, const DWORD y );

  void redraw();

private:
  std::unique_ptr<controller> m_controller;
  safe_font m_font;
};
}  // namespace puzzle15