/*
    Digital Clock: any zoom plugin
    Copyright (C) 2017-2019  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "zoom_dialog.h"
#include "ui_zoom_dialog.h"

namespace any_zoom {

ZoomDialog::ZoomDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::ZoomDialog)
{
  ui->setupUi(this);
  connect(ui->x_zoom_slider, &QSlider::valueChanged, this, &ZoomDialog::onZoomValueChanged);
  connect(ui->y_zoom_slider, &QSlider::valueChanged, this, &ZoomDialog::onZoomValueChanged);
}

ZoomDialog::~ZoomDialog()
{
  delete ui;
}

void ZoomDialog::setZoom(const skin_draw::Zoom& zoom)
{
  if (m_zoom == zoom) return;

  ui->x_zoom_edit->setValue(zoom.zoomX() * 100);
  ui->y_zoom_edit->setValue(zoom.zoomY() * 100);

  m_zoom = zoom;
  emit zoomChanged(m_zoom);
}

void ZoomDialog::onZoomValueChanged()
{
  emit zoomChanged(::skin_draw::Zoom(ui->x_zoom_edit->value() / 100., ui->y_zoom_edit->value() / 100.));
}

} // namespace any_zoom
