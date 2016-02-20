#include "clock_display.h"

#include <QTime>
#include <QLocale>
#include <QRegExp>

namespace digital_clock {
namespace gui {

ClockDisplay::ClockDisplay(QWidget* parent)
  : QLabel(parent), sep_visible_(false), sep_flashes_(true) {
  setAlignment(Qt::AlignCenter);
}

ClockDisplay::~ClockDisplay() {
}

void ClockDisplay::DrawImage(const QImage& image) {
  setPixmap(QPixmap::fromImage(image));
  adjustSize();
  emit changed();
}

void ClockDisplay::SetSeparatorFlash(bool set) {
  sep_flashes_ = set;
  sep_visible_ = !set;
}

void ClockDisplay::SetTimeFormat(const QString& format) {
  time_format_ = format;
  seps_ = format;
  seps_.remove(QRegExp("[hmszap]", Qt::CaseInsensitive));
  emit SeparatorsChanged(seps_);
  TimeoutHandler(); // to emit redraw request
}

void ClockDisplay::TimeoutHandler() {
  if (time_format_.isEmpty()) {
    QString sys_time_format = QLocale::system().timeFormat();
    int sep_pos = sys_time_format.indexOf(':');
    QString time_format = sys_time_format.mid(0, sys_time_format.indexOf(':', sep_pos + 1));

    if (sys_time_format.contains('A', Qt::CaseInsensitive)) {
      time_format += 'A';
    }
    SetTimeFormat(time_format);
  }
  QString str_time = QTime::currentTime().toString(time_format_);

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

} // namespace gui
} // namespace digital_clock
