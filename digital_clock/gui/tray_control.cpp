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
#include <QApplication>

#ifdef Q_OS_MACOS
#define MIcon(x)     QIcon(x).pixmap(16)
#else
#define MIcon(x)     QIcon(x)
#endif

namespace digital_clock {
namespace gui {

TrayControl::TrayControl(QWidget* parent) : QObject(parent)
{
  QMenu* tray_menu = new QMenu(parent);
  // *INDENT-OFF*
  show_hide_action_ = tray_menu->addAction(MIcon(":/clock/images/clock.svg"), tr("&Hide Clock"),
                                           this, SLOT(ShowHideHandler()));
  // *INDENT-ON*
  show_hide_action_->setData(false);
  tray_menu->addSeparator();
  tray_menu->addAction(MIcon(":/clock/images/settings.svg"), tr("&Settings"),
                       this, SIGNAL(ShowSettingsDlg()));
  tray_menu->addAction(MIcon(":/clock/images/about.svg"), tr("&About"),
                       this, SIGNAL(ShowAboutDlg()));
  tray_menu->addSeparator();
  tray_menu->addAction(MIcon(":/clock/images/update.svg"), tr("&Update"),
                       this, SIGNAL(CheckForUpdates()));
  tray_menu->addSeparator();
  tray_menu->addAction(MIcon(":/clock/images/quit.svg"), tr("&Quit"),
                       this, SIGNAL(AppExit()));

  tray_icon_ = new QSystemTrayIcon(QApplication::windowIcon(), this);
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

void TrayControl::ShowHideHandler()
{
  Q_ASSERT(show_hide_action_);
  bool widget_will_visible = show_hide_action_->data().toBool();
  if (widget_will_visible) {
    show_hide_action_->setText(tr("&Hide Clock"));
  } else {
    show_hide_action_->setText(tr("S&how Clock"));
  }
  show_hide_action_->setData(!widget_will_visible);
  emit VisibilityChanged(widget_will_visible);
}

} // namespace gui
} // namespace digital_clock
