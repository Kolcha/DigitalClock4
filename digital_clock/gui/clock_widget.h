/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2014-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_GUI_CLOCK_WIDGET_H
#define DIGITAL_CLOCK_GUI_CLOCK_WIDGET_H

#include <QWidget>

#include <QColor>
#include <QImage>

#include "iskin.h"
#include "settings_keys.h"

namespace skin_draw {
class SkinDrawer;
}

namespace digital_clock {
namespace gui {

class ClockDisplay;

class ClockWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ClockWidget(QWidget* parent = 0);
  ClockDisplay* GetDisplay() const;

  bool preview() const { return preview_mode_; }

signals:
  void SeparatorsChanged(const QString& seps);

public slots:
  void ApplySkin(::skin_draw::ISkin::SkinPtr skin);
  void ApplyOption(Option option, const QVariant& value);
  void EnablePreviewMode();
  void DisablePreviewMode();

  void TimeoutHandler();

private slots:
  void DrawImage(const QImage& image);

private:
  ClockDisplay* display_;
  ::skin_draw::SkinDrawer* drawer_;

  QColor colorize_color_;
  qreal colorize_level_;
  bool colorize_enabled_;
  bool preview_mode_;

  QImage last_image_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CLOCK_WIDGET_H
