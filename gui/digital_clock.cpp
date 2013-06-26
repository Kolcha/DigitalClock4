#include <QHBoxLayout>
#include <QTime>
#include "digital_clock.h"

DigitalClock::DigitalClock(QWidget *parent)
  : QWidget(parent) {
  display_ = new QLabel();
  QHBoxLayout* main_layout = new QHBoxLayout();
  main_layout->addWidget(display_);
  setLayout(main_layout);

  timer_.setTimerType(Qt::PreciseTimer);
  timer_.setInterval(500);
  connect(&timer_, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));
  timer_.start();

  sep_state_ = false;
}

DigitalClock::~DigitalClock() {
  timer_.stop();
}

void DigitalClock::DrawImage(const QImage& image) {
  display_->setPixmap(QPixmap::fromImage(image));
}

void DigitalClock::TimeoutHandler() {
  QString time = QTime::currentTime().toString("hh:mm");
  time[2] = sep_state_ ? ':' : ' ';
  sep_state_ = !sep_state_;
  emit ImageNeeded(time);
}
