#pragma once

#include "puzzle15/platform/handles.h"
#include "puzzle15/platform/window.h"

#include <functional>
#include <memory>

namespace puzzle15
{
struct controller;
struct hdc_canvas;
struct game_stats;
class game_widget : public base_window
{
public:
  game_widget( HINSTANCE instance, HWND parent );
  ~game_widget();

  void reset_game();
  rect_size calc_border_to_fit( rect_size available_size ) const;

  using status_updater = std::function<void( const game_stats& )>;
  void set_status_updater( const status_updater& updater );

private:
  virtual LRESULT CALLBACK wnd_proc( UINT msg, WPARAM wParam, LPARAM lParam ) override;

  LRESULT on_create();
  LRESULT on_paint();
  LRESULT on_resize( WORD width, WORD height );
  LRESULT on_left_mouse_btn_down( DWORD mouseX, DWORD mouseY );
  LRESULT on_left_mouse_btn_up( DWORD mouseX, DWORD mouseY );
  LRESULT on_right_mouse_btn_up( DWORD mouseX, DWORD mouseY );
  LRESULT on_timer( WPARAM timer_id );

  hdc_canvas make_canvas( const PAINTSTRUCT& ps, HDC dc, size_t cell_side ) const;


  rect_size calc_size_to_fit( rect_size available_size ) const;
  size_t calc_max_cell_side_to_fit( rect_size available_size ) const;
  size_t calc_current_cell_side() const;

  cell_indices get_cell_index_by_coords( DWORD mouseX, DWORD mouseY ) const;
  RECT get_cell_rect_by_index( cell_indices idx ) const;

  void redraw();
  void redraw_cell( cell_indices c );

  void move_cell( cell_indices from, cell_indices to );

  void update_game_stats();

private:
  std::unique_ptr<controller> m_controller;
  safe_font m_font;
  status_updater m_status_updater;
};
}  // namespace puzzle15