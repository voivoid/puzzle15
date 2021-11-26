#include "stdafx.h"

#include "puzzle15/platform/thunk.h"

#include "puzzle15/assertions.h"

namespace
{
constexpr std::byte operator"" _b( unsigned long long b )
{
  p15_assert( b <= 255 );
  return std::byte( b );
}
}  // namespace

namespace puzzle15
{
void thunk::free_trampoline::operator()( void* p ) const
{
  bool success = ::VirtualFree( p, 0, MEM_RELEASE );
  p15_assert( success );
}

std::pair<WNDPROC, WNDPROC> thunk::generate_callbacks( const base_window* const window, HWND* const wnd_ptr )
{
#ifdef _M_X64
  static const std::byte function_code[] = {
    // clang-format off
    
    // initial wndproc; it stores hwnd argument to m_wnd; as soon as m_wnd is initialzed base wndproc can be used
    0x48_b, 0xB8_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b,  // movabs rax, &wnd_ptr
    0x48_b, 0x89_b, 0x08_b,                                                          // mov QWORD PTR [rax],rcx

    // base wndproc; make a 'base_window::wnd_proc' virtual call
    0x48_b, 0xB9_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b,  // movabs rcx, &window
    0x48_b, 0x8B_b, 0x01_b,                                                          // mov rax,[rcx]
    0xff_b, 0x20_b                                                                   // jmp [rax]       ( virtual call )
    
  };  // clang-format on

  std::byte* code = reinterpret_cast<std::byte*>( ::VirtualAlloc( NULL, sizeof( function_code ), MEM_COMMIT, PAGE_EXECUTE_READWRITE ) );
  p15_ensure( code );

  m_trampoline.reset( code );

  memcpy( code, function_code, sizeof( function_code ) );
  memcpy( code + 2, &wnd_ptr, sizeof( void* ) );
  memcpy( code + 15, &window, sizeof( void* ) );

  p15_ensure( ::FlushInstructionCache( ::GetCurrentProcess(), NULL, 0 ) );

  return { reinterpret_cast<WNDPROC>( code ), reinterpret_cast<WNDPROC>( code + 13 ) };
#else
#  error unsupported platform
#endif
}
}  // namespace puzzle15