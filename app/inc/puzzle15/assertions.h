#pragma once

#include <cassert>
#include <stdexcept>

#if defined( P15_ENSURE )
#  error P15_ENSURE redefinition
#endif

#ifdef NDEBUG
#  define p15_ensure( expr )                                                                                                               \
    {                                                                                                                                      \
      if ( !( expr ) )                                                                                                                     \
      {                                                                                                                                    \
        throw std::runtime_error( #expr " failed" );                                                                                       \
      }                                                                                                                                    \
    };
#else
#  define p15_ensure assert
#endif