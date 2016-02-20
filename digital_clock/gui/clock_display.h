#ifndef DIGITAL_CLOCK_GUI_CLOCK_DISPLAY_H
#define DIGITAL_CLOCK_GUI_CLOCK_DISPLAY_H

#include <QLabel>

namespace digital_clock {
namespace gui {

class ClockDisplay : public QLabel {
  Q_OBJECT

public:
  explicit ClockDisplay(QWidget* parent = 0);
  ~ClockDisplay();

signals:
  void ImageNeeded(const QString& text);
  void SeparatorsChanged(const QString& seps);
  void changed();

public slots:
  void DrawImage(const QImage& image);
  void SetSeparatorFlash(bool set);
  void SetTimeFormat(const QString& format);

  void TimeoutHandler();

private:
  bool sep_visible_;
  bool sep_flashes_;
  QString time_format_;
  QString seps_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CLOCK_DISPLAY_H
