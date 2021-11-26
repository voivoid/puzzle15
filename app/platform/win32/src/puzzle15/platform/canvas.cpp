#include "stdafx.h"

#include "puzzle15/platform/canvas.h"

#include "puzzle15/assertions.h"

namespace puzzle15
{
hdc_canvas::hdc_canvas( const HDC dc, const rect_size size, const size_t cell_side ) : m_dc( dc ), m_size( size ), m_cell_side( cell_side )
{
}

void hdc_canvas::draw_rect( const rectangle& r )
{
  auto success = ::Rectangle( m_dc, r.left_top.x, r.left_top.y, r.right_bottom.x, r.right_bottom.y );
  p15_assert( success );
}

void hdc_canvas::draw_text( const rectangle& text_rect, const std::wstring& text )
{
  RECT r;
  r.left   = text_rect.left_top.x;
  r.top    = text_rect.left_top.y;
  r.right  = text_rect.right_bottom.x;
  r.bottom = text_rect.right_bottom.y;

  bool success = ::DrawText( m_dc, text.c_str(), static_cast<int>( text.size() ), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
  p15_assert( success );
}

size_t hdc_canvas::width() const
{
  return m_size.width;
}

size_t hdc_canvas::height() const
{
  return m_size.height;
}

size_t hdc_canvas::cell_side() const
{
  return m_cell_side;
}

}  // namespace puzzle15