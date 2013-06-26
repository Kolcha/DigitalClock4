#include <QHBoxLayout>
#include <QTime>
#include "digital_clock.h"

DigitalClock::DigitalClock(QWidget *parent)
  : QWidget(parent) {
  display_ = new QLabel();
  QHBoxLayout* main_layout = new QHBoxLayout();
  main_layout->addWidget(display_);
  setLayout(main_layout);

  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);

  timer_.setTimerType(Qt::PreciseTimer);
  timer_.setInterval(500);
  connect(&timer_, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));
  timer_.start();

  sep_visible_ = false;
  sep_flashes_ = true;
}

DigitalClock::~DigitalClock() {
  timer_.stop();
}

void DigitalClock::DrawImage(const QImage& image) {
  display_->setPixmap(QPixmap::fromImage(image));
}

void DigitalClock::SetStaysOnTop(bool set) {
  SetWindowFlag(Qt::WindowStaysOnTopHint, set);
}

void DigitalClock::SetTransparentForMouse(bool set) {
  SetWindowFlag(Qt::WindowTransparentForInput, set);
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

void DigitalClock::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  hide();
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}
