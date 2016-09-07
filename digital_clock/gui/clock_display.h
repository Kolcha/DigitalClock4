#ifndef DIGITAL_CLOCK_GUI_CLOCK_DISPLAY_H
#define DIGITAL_CLOCK_GUI_CLOCK_DISPLAY_H

#include <QLabel>

namespace digital_clock {
namespace gui {

class ClockDisplay : public QLabel
{
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

  void SetURLEnabled(bool enable);
  void SetURL(const QString& url);

  void TimeoutHandler();

protected:
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

private:
  bool sep_visible_;
  bool sep_flashes_;
  QString time_format_;
  QString seps_;

  bool url_enabled_;
  QString url_string_;

  QPoint drag_position_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CLOCK_DISPLAY_H
