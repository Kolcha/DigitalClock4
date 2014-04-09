#ifndef DIGITAL_CLOCK_GUI_CLOCK_WIDGET_H
#define DIGITAL_CLOCK_GUI_CLOCK_WIDGET_H

#include <QWidget>
#include "iskin.h"
#include "settings_keys.h"

namespace skin_draw {
class SkinDrawer;
}

namespace digital_clock {
namespace gui {

class ClockDisplay;

class ClockWidget : public QWidget {
  Q_OBJECT

public:
  explicit ClockWidget(QWidget* parent = 0);
  ClockDisplay* GetDisplay() const;

signals:
  void PositionChanged(const QPoint& new_pos);
  void SeparatorsChanged(const QString& seps);

public slots:
  void ApplySkin(::skin_draw::ISkin::SkinPtr skin);
  void ApplyOption(Options option, const QVariant& value);

protected:
  void mouseMoveEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

private:
  void SetWindowFlag(Qt::WindowFlags flag, bool set);

  ClockDisplay* display_;
  ::skin_draw::SkinDrawer* drawer_;
  QPoint drag_position_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CLOCK_WIDGET_H
