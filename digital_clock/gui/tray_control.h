/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_GUI_TRAY_CONTROL_H
#define DIGITAL_CLOCK_GUI_TRAY_CONTROL_H

#include <QObject>
#include <QSystemTrayIcon>

class QAction;

namespace digital_clock {
#ifdef Q_OS_WIN
class SystemThemeTracker;
#endif
namespace gui {

class ContextMenu;

class TrayControl : public QObject
{
  Q_OBJECT

public:
  explicit TrayControl(QObject* parent = nullptr);
  ~TrayControl();

  QSystemTrayIcon* GetTrayIcon() const Q_DECL_NOEXCEPT;
  QAction* GetShowHideAction() const Q_DECL_NOEXCEPT;

signals:
  void VisibilityChanged(bool visible);
  void PositionChanged(Qt::Alignment pos);
  void ShowSettingsDlg();
  void ShowAboutDlg();
  void CheckForUpdates();
  void AppExit();

private slots:
  void TrayEventHandler(QSystemTrayIcon::ActivationReason reason);
#ifdef Q_OS_WIN
  void SysThemeChangedHandler(bool is_light_theme);
#endif

private:
  QIcon platform_tray_icon() const;

  QSystemTrayIcon* tray_icon_;
  ContextMenu* tray_menu_;
#ifdef Q_OS_WIN
  SystemThemeTracker* sys_theme_tracker_;
#endif
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_TRAY_CONTROL_H
