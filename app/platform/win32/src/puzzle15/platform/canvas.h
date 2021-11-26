#pragma once

#include "puzzle15/geo.h"
#include "puzzle15/platform_ifaces.h"

namespace puzzle15
{
struct hdc_canvas : canvas
{
  hdc_canvas( HDC dc, rect_size size, size_t cell_side );

  virtual void draw_rect( const rectangle& r ) override;
  virtual void draw_text( const rectangle& text_rect, const std::wstring& text ) override;

  virtual size_t width() const override;
  virtual size_t height() const override;
  virtual size_t cell_side() const override;

private:
  HDC m_dc;
  rect_size m_size;
  size_t m_cell_side;
};
}  // namespace puzzle15