/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2014-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_GUI_CLOCK_DISPLAY_H
#define DIGITAL_CLOCK_GUI_CLOCK_DISPLAY_H

#include <QLabel>

#include <QTimeZone>

namespace digital_clock {
namespace gui {

QString time_to_str(const QTime& t, const QString& fmt);

class ClockDisplay : public QLabel
{
  Q_OBJECT

public:
  explicit ClockDisplay(QWidget* parent = nullptr);

signals:
  void ImageNeeded(const QString& text);
  void SeparatorsChanged(const QString& seps);

public slots:
  void DrawImage(const QImage& image);
  void SetSeparatorFlash(bool set);
  void SetTimeFormat(const QString& format);
  void SetDisplayLocalTime(bool set);
  void SetTimeZone(const QTimeZone& tz);

  void SetURLEnabled(bool enable);
  void SetURL(const QString& url);

  void TimeoutHandler();

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

private:
  bool sep_visible_;
  bool sep_flashes_;
  QString time_format_;
  QString seps_;

  bool url_enabled_;
  QString url_string_;

  QPoint drag_position_;

  bool local_time_;
  QTimeZone time_zone_;
  QString sys_time_format_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CLOCK_DISPLAY_H
