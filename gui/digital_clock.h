#ifndef DIGITAL_CLOCK_H
#define DIGITAL_CLOCK_H

#include <QWidget>
#include <QLabel>

class DigitalClock : public QWidget {
  Q_OBJECT
public:
  explicit DigitalClock(QWidget *parent = 0);

signals:

public slots:

private:
  QLabel* clock_;
};

#endif // DIGITAL_CLOCK_H
