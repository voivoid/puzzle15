#pragma once

#include <memory>
#include <utility>

namespace puzzle15
{
class base_window;
class thunk
{
public:
  std::pair<WNDPROC, WNDPROC> generate_callbacks( const base_window* window, HWND* wnd_ptr );

private:
  struct free_trampoline
  {
    void operator()( void* ) const;
  };
  std::unique_ptr<void, free_trampoline> m_trampoline;
};
}  // namespace puzzle15