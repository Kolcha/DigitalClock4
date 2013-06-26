#ifndef DIGITAL_CLOCK_H
#define DIGITAL_CLOCK_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class DigitalClock : public QWidget {
  Q_OBJECT
public:
  explicit DigitalClock(QWidget *parent = 0);
  ~DigitalClock();

signals:
  void ImageNeeded(const QString& text);

public slots:
  void DrawImage(const QImage& image);

private slots:
  void TimeoutHandler();

private:
  QLabel* display_;
  QTimer timer_;
  bool sep_state_;
};

#endif // DIGITAL_CLOCK_H
