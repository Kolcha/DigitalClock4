/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QMenu>
#include <QIcon>
#ifdef Q_OS_MACOS
#include <QSysInfo>
#include <QVersionNumber>
#endif
#include <QApplication>

namespace digital_clock {
namespace gui {

TrayControl::TrayControl(QWidget* parent) : QObject(parent)
{
  QMenu* tray_menu = new QMenu(parent);
  show_hide_action_ = tray_menu->addAction(tr("&Visible"), this, SIGNAL(VisibilityChanged(bool)));
  show_hide_action_->setCheckable(true);
  show_hide_action_->setChecked(true);
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/settings.svg.p"), tr("&Settings"),
                       this, SIGNAL(ShowSettingsDlg()));
  tray_menu->addAction(QIcon(":/clock/images/info.svg.p"), tr("&About"),
                       this, SIGNAL(ShowAboutDlg()));
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/update.svg.p"), tr("&Update"),
                       this, SIGNAL(CheckForUpdates()));
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/quit.svg.p"), tr("&Quit"),
                       this, SIGNAL(AppExit()));

  QIcon tray_icon(":/clock/images/clock.svg");
#ifdef Q_OS_MACOS
  if (QVersionNumber::fromString(QSysInfo::productVersion()) >= QVersionNumber(10, 10))
#endif
    tray_icon.setIsMask(true);
  tray_icon_ = new QSystemTrayIcon(tray_icon, this);
  tray_icon_->setVisible(true);
  tray_icon_->setContextMenu(tray_menu);
  tray_icon_->setToolTip(QApplication::applicationDisplayName() + " " + QApplication::applicationVersion());
  connect(tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(TrayEventHandler(QSystemTrayIcon::ActivationReason)));
}

QSystemTrayIcon* TrayControl::GetTrayIcon() const
{
  return tray_icon_;
}

QAction* TrayControl::GetShowHideAction() const
{
  return show_hide_action_;
}

void TrayControl::TrayEventHandler(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::DoubleClick) emit ShowSettingsDlg();
}

} // namespace gui
} // namespace digital_clock
