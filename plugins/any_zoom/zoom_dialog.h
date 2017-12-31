/*
    Digital Clock: any zoom plugin
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef ANY_ZOOM_ZOOM_DIALOG_H
#define ANY_ZOOM_ZOOM_DIALOG_H

#include <QDialog>

#include "zoom_type.h"

namespace any_zoom {

namespace Ui {
class ZoomDialog;
}

class ZoomDialog : public QDialog
{
  Q_OBJECT
  Q_PROPERTY(::skin_draw::Zoom zoom READ zoom WRITE setZoom NOTIFY zoomChanged)

public:
  explicit ZoomDialog(QWidget* parent = 0);
  ~ZoomDialog();

  ::skin_draw::Zoom zoom() const Q_DECL_NOEXCEPT { return m_zoom; }

public slots:
  void setZoom(const ::skin_draw::Zoom& zoom);

signals:
  void zoomChanged(const ::skin_draw::Zoom& zoom);

private slots:
  void onZoomValueChanged();

private:
  Ui::ZoomDialog* ui;
  ::skin_draw::Zoom m_zoom;
};

} // namespace any_zoom

#endif // ANY_ZOOM_ZOOM_DIALOG_H
