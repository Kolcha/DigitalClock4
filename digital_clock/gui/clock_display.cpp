/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "clock_display.h"

#include <QTime>
#include <QLocale>
#include <QRegExp>
#include <QMouseEvent>
#include <QDesktopServices>

namespace digital_clock {
namespace gui {

ClockDisplay::ClockDisplay(QWidget* parent) :
  QLabel(parent),
  sep_visible_(false), sep_flashes_(true)
{
  setAlignment(Qt::AlignCenter);
}

ClockDisplay::~ClockDisplay()
{
}

void ClockDisplay::DrawImage(const QImage& image)
{
  setPixmap(QPixmap::fromImage(image));
  adjustSize();
  emit changed();
}

void ClockDisplay::SetSeparatorFlash(bool set)
{
  sep_flashes_ = set;
  sep_visible_ = !set;
}

void ClockDisplay::SetTimeFormat(const QString& format)
{
  time_format_ = format;
  seps_ = format;
  seps_.remove(QRegExp("[hmszap]", Qt::CaseInsensitive));
  emit SeparatorsChanged(seps_);
  TimeoutHandler(); // to emit redraw request
}

void ClockDisplay::SetURLEnabled(bool enable)
{
  url_enabled_ = enable;
}

void ClockDisplay::SetURL(const QString& url)
{
  url_string_ = url;
}

void ClockDisplay::TimeoutHandler()
{
  if (time_format_.isEmpty()) {
    QString sys_time_format = QLocale::system().timeFormat();
    int sep_pos = sys_time_format.indexOf(':');
    QString time_format = sys_time_format.mid(0, sys_time_format.indexOf(':', sep_pos + 1));

    if (sys_time_format.contains('A', Qt::CaseInsensitive)) {
      time_format += 'A';
    }
    SetTimeFormat(time_format);
  }
  QLocale c_locale(QLocale::C, QLocale::AnyCountry);
  QString str_time = c_locale.toString(QTime::currentTime(), time_format_);

  QList<int> seps_pos;
  for (int i = 0; i < str_time.length(); ++i) {
    if (seps_.contains(str_time[i], Qt::CaseInsensitive)) seps_pos.append(i);
  }

  if (sep_flashes_) {
    for (auto& sep_pos : seps_pos) {
      if (!sep_visible_) str_time[sep_pos] = ' ';
    }
    sep_visible_ = !sep_visible_;
  }
  emit ImageNeeded(str_time);
}

void ClockDisplay::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    drag_position_ = event->globalPos();
  }
  event->ignore();
}

void ClockDisplay::mouseReleaseEvent(QMouseEvent* event)
{
  if (url_enabled_ && event->button() == Qt::LeftButton &&
      event->globalPos() == drag_position_) {
    QDesktopServices::openUrl(url_string_);
  }
  event->ignore();
}

} // namespace gui
} // namespace digital_clock
