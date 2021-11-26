#include "stdafx.h"

#include "puzzle15/platform/error.h"

#include "puzzle15/assertions.h"

#include <stdexcept>
#include <string>


namespace puzzle15
{

struct last_error_exception : std::exception
{
  last_error_exception( const char* const msg ) : m_msg( msg ), m_last_error( ::GetLastError() )
  {
  }

  virtual char const* what() const override
  {
    // TODO: format m_last_error message
    return m_msg.c_str();
  }

  std::string m_msg;
  DWORD m_last_error;
};

void throw_last_error( const char* const msg )
{
  throw last_error_exception{ msg };
}

void show_exception_message( const char* const message )
{
  bool success = ::MessageBoxA( nullptr, message, "Unhandled exception", MB_OK | MB_ICONERROR ) != 0;
  p15_assert( success );
}

}  // namespace puzzle15