#include <QTime>
#include "digital_clock.h"

DigitalClock::DigitalClock(QWidget* parent)
  : QLabel(parent) {
  timer_ = new QTimer(this);
  timer_->setInterval(500);
  connect(timer_, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));
  timer_->start();

  sep_visible_ = false;
  sep_flashes_ = true;
}

DigitalClock::~DigitalClock() {
  timer_->stop();
}

void DigitalClock::DrawImage(const QImage& image) {
  setPixmap(QPixmap::fromImage(image));
}

void DigitalClock::SetSeparatorFlash(bool set) {
  sep_flashes_ = set;
  sep_visible_ = !set;
}

void DigitalClock::TimeoutHandler() {
  QString time = QTime::currentTime().toString("hh:mm");
  if (sep_flashes_) {
    time[2] = sep_visible_ ? ':' : ' ';
    sep_visible_ = !sep_visible_;
  }
  emit ImageNeeded(time);
}
