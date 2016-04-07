#ifndef DIGITAL_CLOCK_GUI_LED_TOOL_BUTTON_H
#define DIGITAL_CLOCK_GUI_LED_TOOL_BUTTON_H

#include <QToolButton>
#include <QColor>

namespace digital_clock {
namespace gui {

class LedToolButton : public QToolButton
{
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor)

public:
  explicit LedToolButton(QWidget* parent = 0);

  QColor color() const;

signals:
  void colorChanged(const QColor& color);

public slots:
  void setColor(const QColor& color);

private:
  QColor color_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_LED_TOOL_BUTTON_H
