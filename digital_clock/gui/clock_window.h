/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DIGITAL_CLOCK_GUI_CLOCK_WINDOW_H
#define DIGITAL_CLOCK_GUI_CLOCK_WINDOW_H

#include <QWidget>

#include "settings_keys.h"
#include "iskin.h"

#include "gui/hover_buttons.h"

namespace digital_clock {

namespace core {
class ClockSettings;
class ClockState;
}

namespace gui {
class ClockWidget;
class ContextMenu;


class ClockWindow : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(bool previewMode READ previewMode WRITE setPreviewMode)

public:
  explicit ClockWindow(core::ClockSettings* app_config, int id = 0, QWidget* parent = nullptr);

  bool previewMode() const;

  gui::ClockWidget* clockWidget() const Q_DECL_NOEXCEPT { return clock_widget_; }
  gui::ContextMenu* contextMenu() const Q_DECL_NOEXCEPT { return c_menu_; }

protected:
  void showEvent(QShowEvent* event) override;
  void hideEvent(QHideEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void moveEvent(QMoveEvent* event) override;
  void contextMenuEvent(QContextMenuEvent* event) override;

public slots:
  void ApplyOption(const Option opt, const QVariant& value);
  void ApplySkin(skin_draw::ISkin::SkinPtr skin);

  void ChangeVisibility(bool visible);
  void EnsureVisible();
  void RestoreVisibility();

  void EnablePreviewMode();
  void DisablePreviewMode();
  void setPreviewMode(bool enable);

  void TimeoutHandler();

  void LoadState();
  void SaveState();

  void MoveWindow(Qt::Alignment align);

private slots:
  void onHoverButtonClicked(HoverButtons::Direction direction);
  void CorrectPosition();

private:
  void CorrectPositionImpl();
#ifdef Q_OS_WIN
  void WinOnTopWorkaround();
  void KeepOnDesktop();
#endif
  void SetWindowFlag(Qt::WindowType flag, bool set);

  void SetVisibleOnAllDesktops(bool set);

  core::ClockSettings* app_config_;
  core::ClockState* state_;

  gui::ClockWidget* clock_widget_;
  gui::ContextMenu* c_menu_;

  const int id_;

  bool dragging_;
  QPoint drag_position_;
  CAlignment cur_alignment_ = A_LEFT;
  bool last_visibility_;

  QColor bg_color_;
  bool show_border_ = true;

  bool fullscreen_detect_enabled_;
  bool keep_always_visible_;
  QStringList window_ignore_list_;

  bool snap_to_edges_ = true;
  int snap_threshold_ = 20;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CLOCK_WINDOW_H
