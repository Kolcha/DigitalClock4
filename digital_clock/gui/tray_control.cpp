/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "tray_control.h"

#include <QIcon>
#ifdef Q_OS_WIN
#include <QSysInfo>
#include <QVersionNumber>
#endif
#include <QApplication>

#include "gui/context_menu.h"

namespace digital_clock {
namespace gui {

TrayControl::TrayControl(QObject* parent) : QObject(parent)
{
  tray_menu_ = new ContextMenu();
  connect(tray_menu_, &ContextMenu::VisibilityChanged, this, &TrayControl::VisibilityChanged);
  connect(tray_menu_, &ContextMenu::PositionChanged, this, &TrayControl::PositionChanged);
  connect(tray_menu_, &ContextMenu::ShowSettingsDlg, this, &TrayControl::ShowSettingsDlg);
  connect(tray_menu_, &ContextMenu::ShowAboutDlg, this, &TrayControl::ShowAboutDlg);
  connect(tray_menu_, &ContextMenu::CheckForUpdates, this, &TrayControl::CheckForUpdates);
  connect(tray_menu_, &ContextMenu::AppExit, this, &TrayControl::AppExit);

#ifdef Q_OS_WIN
  QIcon tray_icon;
  if (QVersionNumber::fromString(QSysInfo::productVersion()) >= QVersionNumber(10))
    tray_icon = QIcon(":/clock/icons/tray/clock-alt.svg");
  else
    tray_icon = QIcon(":/clock/icons/tray/clock.svg");
#else
  QIcon tray_icon(":/clock/icons/tray/clock-smaller.svg");
#endif
  tray_icon.setIsMask(true);
  tray_icon_ = new QSystemTrayIcon(tray_icon, this);
  tray_icon_->setVisible(true);
  tray_icon_->setContextMenu(tray_menu_->menu());
  tray_icon_->setToolTip(QApplication::applicationDisplayName() + " " + QApplication::applicationVersion());
  connect(tray_icon_, &QSystemTrayIcon::activated, this, &TrayControl::TrayEventHandler);
}

TrayControl::~TrayControl()
{
  delete tray_menu_;
}

QSystemTrayIcon* TrayControl::GetTrayIcon() const Q_DECL_NOEXCEPT
{
  return tray_icon_;
}

QAction* TrayControl::GetShowHideAction() const Q_DECL_NOEXCEPT
{
  return tray_menu_->visibilityAction();
}

void TrayControl::TrayEventHandler(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::DoubleClick) emit ShowSettingsDlg();
}

} // namespace gui
} // namespace digital_clock
