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

#include "clock_display.h"

#include <QDateTime>
#include <QLocale>
#include <QRegExp>
#include <QMouseEvent>
#include <QDesktopServices>

namespace digital_clock {
namespace gui {


static QString time_to_str(const QTime& t, const QString& fmt)
{
  QString out_str;
  for (int i = 0; i < fmt.length();) {
    QStringRef ss = fmt.midRef(i, 2);
    if (ss == "HH") {
      out_str += QString("%1").arg(t.hour(), 2, 10, QLatin1Char('0'));
      i += 2;
      continue;
    }
    if (ss == "hh") {
      int hour = t.hour();
      if (hour == 0) hour = 12;
      if (hour > 12) hour -= 12;
      out_str += QString("%1").arg(hour, 2, 10, QLatin1Char('0'));
      i += 2;
      continue;
    }
    if (ss == "mm") {
      out_str += QString("%1").arg(t.minute(), 2, 10, QLatin1Char('0'));
      i += 2;
      continue;
    }
    if (ss == "ss") {
      out_str += QString("%1").arg(t.second(), 2, 10, QLatin1Char('0'));
      i += 2;
      continue;
    }
    if (ss == "AP") {
      out_str += t.hour() < 12 ? "AM" : "PM";
      i += 2;
      continue;
    }
    if (ss == "ap") {
      out_str += t.hour() < 12 ? "am" : "pm";
      i += 2;
      continue;
    }
    if (fmt[i] == 'H') {
      out_str += QString::number(t.hour());
      ++i;
      continue;
    }
    if (fmt[i] == 'h') {
      int hour = t.hour();
      if (hour == 0) hour = 12;
      if (hour > 12) hour -= 12;
      out_str += QString::number(hour);
      ++i;
      continue;
    }
    if (fmt[i] == 'm') {
      out_str += QString::number(t.minute());
      ++i;
      continue;
    }
    if (fmt[i] == 's') {
      out_str += QString::number(t.second());
      ++i;
      continue;
    }
    if (fmt[i] == 'A') {
      out_str += t.hour() < 12 ? "AM" : "PM";
      ++i;
      continue;
    }
    if (fmt[i] == 'a') {
      out_str += t.hour() < 12 ? "am" : "pm";
      ++i;
      continue;
    }
    out_str += fmt[i++];
  }
  return out_str;
}


ClockDisplay::ClockDisplay(QWidget* parent) :
  QLabel(parent),
  sep_visible_(false), sep_flashes_(true), url_enabled_(false),
  local_time_(true), time_zone_(QTimeZone::systemTimeZone())
{
  setAlignment(Qt::AlignCenter);
}

void ClockDisplay::DrawImage(const QImage& image)
{
  setPixmap(QPixmap::fromImage(image));
}

void ClockDisplay::SetSeparatorFlash(bool set) noexcept
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

void ClockDisplay::SetDisplayLocalTime(bool set)
{
  local_time_ = set;
  TimeoutHandler();
}

void ClockDisplay::SetTimeZone(const QTimeZone& tz)
{
  time_zone_ = tz;
  TimeoutHandler();
}

void ClockDisplay::SetURLEnabled(bool enable)
{
  url_enabled_ = enable;
  setCursor(enable ? Qt::PointingHandCursor : Qt::ArrowCursor);
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

  QDateTime dt = QDateTime::currentDateTime();
  if (!local_time_) dt = dt.toTimeZone(time_zone_);
  QString str_time = time_to_str(dt.time(), time_format_);

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
