#include <QTime>
#include <QLocale>
#include <QRegExp>
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

  QString seps = time_format_;
  seps.remove(QRegExp("[hmszap]", Qt::CaseInsensitive));
  QList<int> seps_pos;
  for (int i = 0; i < time_format_.length(); ++i) {
    if (seps.contains(time_format_[i], Qt::CaseInsensitive)) seps_pos.append(i);
  }

  if (sep_flashes_) {
    for (auto& sep_pos : seps_pos) {
      if (!sep_visible_) str_time[sep_pos] = ' ';
    }
    sep_visible_ = !sep_visible_;
  }
  emit ImageNeeded(str_time);
}
