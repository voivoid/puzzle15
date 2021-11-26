#pragma once

#include <cassert>
#include <stdexcept>

#if defined( p15_ensure )
#  error "p15_ensure redefinition"
#else
#  ifdef NDEBUG
#    define p15_ensure( expr )                                                                                                             \
      {                                                                                                                                    \
        if ( !( expr ) )                                                                                                                   \
        {                                                                                                                                  \
          throw std::runtime_error( #expr " failed" );                                                                                     \
        }                                                                                                                                  \
      };
#  else
#    define p15_ensure assert
#  endif
#endif


#if defined( p15_assert )
#  error "p15_assert redefinition"
#else
#  ifdef NDEBUG
#    define p15_assert( expr ) (void)expr
#  else
#    define p15_assert assert
#  endif
#endif