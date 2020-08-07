#include "stdafx.h"

#include "puzzle15/assertions.h"
#include "puzzle15/platform/canvas.h"

namespace puzzle15
{
hdc_canvas::hdc_canvas( HDC dc, size cell_size ) : m_dc( dc ), m_size( cell_size )
{
}

void hdc_canvas::draw_rect( const rect& r )
{
  p15_ensure( ::Rectangle( m_dc, r.left_top.x, r.left_top.y, r.right_bottom.x, r.right_bottom.y ) );
}

void hdc_canvas::draw_text( const rect text_rect, const std::wstring& text )
{
  RECT r;
  r.left   = text_rect.left_top.x;
  r.top    = text_rect.left_top.y;
  r.right  = text_rect.right_bottom.x;
  r.bottom = text_rect.right_bottom.y;

  p15_ensure( ::DrawText( m_dc, text.c_str(), static_cast<int>( text.size() ), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE ) );
}

coord hdc_canvas::width() const
{
  return m_size.width;
}

coord hdc_canvas::height() const
{
  return m_size.height;
}
}  // namespace puzzle15