#include <QTime>
#include <QLocale>
#include "digital_clock.h"

DigitalClock::DigitalClock(QWidget* parent)
  : QLabel(parent) {
  timer_ = new QTimer(this);
  timer_->setInterval(500);
  connect(timer_, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));
  timer_->start();

  sep_visible_ = false;
  sep_flashes_ = true;
  display_am_pm_ = false;
}

DigitalClock::~DigitalClock() {
  timer_->stop();
}

void DigitalClock::DrawImage(const QImage& image) {
  setPixmap(QPixmap::fromImage(image));
  parentWidget()->adjustSize();
}

void DigitalClock::SetSeparatorFlash(bool set) {
  sep_flashes_ = set;
  sep_visible_ = !set;
}

void DigitalClock::SetDisplayAMPM(bool set) {
  display_am_pm_ = set;
}

void DigitalClock::SetTimeFormat(const QString& format) {
  time_format_ = format;
}

void DigitalClock::TimeoutHandler() {
  if (time_format_.isEmpty()) time_format_ = QLocale::system().timeFormat();
  QTime cur_time = QTime::currentTime();
  QString str_time = cur_time.toString(time_format_);
  int sep_pos = str_time.indexOf(':');
  str_time = str_time.mid(0, str_time.lastIndexOf(':'));
  if (display_am_pm_ && time_format_.contains('A', Qt::CaseInsensitive)) {
    str_time += cur_time.toString("AP");
  }
  if (sep_flashes_) {
    str_time[sep_pos] = sep_visible_ ? ':' : ' ';
    sep_visible_ = !sep_visible_;
  }
  emit ImageNeeded(str_time);
}
