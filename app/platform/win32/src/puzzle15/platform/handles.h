#pragma once

#include "puzzle15/assertions.h"

#include <Windows.h>
#include <memory>

namespace puzzle15
{
template <typename T>
struct safe_handle_deleter
{
  typedef T pointer;

  void operator()( const T font ) const
  {
    bool success = ::DeleteObject( font );
    p15_assert( success );
  }
};

template <typename T>
using safe_handle = std::unique_ptr<T, safe_handle_deleter<T>>;

using safe_font = safe_handle<HFONT>;
}  // namespace puzzle15