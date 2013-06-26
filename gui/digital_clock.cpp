#include <QHBoxLayout>
#include "digital_clock.h"

DigitalClock::DigitalClock(QWidget *parent)
  : QWidget(parent) {
  clock_ = new QLabel();
  QHBoxLayout* main_layout = new QHBoxLayout();
  main_layout->addWidget(clock_);
  setLayout(main_layout);
}
