#ifndef DIGITAL_CLOCK_GUI_CLOCK_WIDGET_H
#define DIGITAL_CLOCK_GUI_CLOCK_WIDGET_H

#include <QWidget>

#include <QColor>
#include <QImage>

#include "iskin.h"
#include "settings_keys.h"

namespace skin_draw {
class SkinDrawer;
}

namespace digital_clock {
namespace gui {

class ClockDisplay;

class ClockWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ClockWidget(QWidget* parent = 0);
  ClockDisplay* GetDisplay() const;

  bool preview() const { return preview_mode_; }

signals:
  void SeparatorsChanged(const QString& seps);

  void changed();

public slots:
  void ApplySkin(::skin_draw::ISkin::SkinPtr skin);
  void ApplyOption(Option option, const QVariant& value);
  void EnablePreviewMode();
  void DisablePreviewMode();

  void TimeoutHandler();

private slots:
  void Update();
  void DrawImage(const QImage& image);

private:
  ClockDisplay* display_;
  ::skin_draw::SkinDrawer* drawer_;

  QColor colorize_color_;
  qreal colorize_level_;
  bool colorize_enabled_;
  bool preview_mode_;

  QImage last_image_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CLOCK_WIDGET_H
