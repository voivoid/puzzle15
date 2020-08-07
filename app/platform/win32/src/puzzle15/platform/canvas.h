#pragma once

#include "puzzle15/platform_iface.h"

namespace puzzle15
{
struct hdc_canvas : canvas
{
  hdc_canvas( HDC dc, size cell_size );

  virtual void draw_rect( const rect& r ) override;
  virtual void draw_text( rect text_rect, const std::wstring& text ) override;

  virtual coord width() const override;
  virtual coord height() const override;

private:
  HDC m_dc;
  size m_size;
};
}  // namespace puzzle15