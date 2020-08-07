#pragma once

#include "puzzle15/geo.h"
#include "puzzle15/platform/thunk.h"

#include <memory>

#include <windows.h>


namespace puzzle15
{

class base_window
{
  // this method SHOULD be the first one in the class vtable
  virtual LRESULT CALLBACK wnd_proc( UINT msg, WPARAM wParam, LPARAM lParam ) = 0;

public:
  void destroy();
  void show( int cmd_show );
  void move( point p, size s );
  size get_client_size() const;

protected:
  base_window();
  virtual ~base_window();

  void register_and_create( WNDCLASSEX& wcx,
                            DWORD style,
                            DWORD ex_style,
                            LPCWSTR window_name,
                            int x,
                            int y,
                            int width,
                            int height,
                            HWND parent,
                            HINSTANCE instance );

protected:
  HWND m_wnd;
  thunk m_thunk;
};

}  // namespace puzzle15