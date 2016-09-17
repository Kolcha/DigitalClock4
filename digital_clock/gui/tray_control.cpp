#include "tray_control.h"

#include <QMenu>
#include <QIcon>
#include <QApplication>

#ifdef Q_OS_OSX
#define MIcon(x)     QIcon(x).pixmap(16)
#else
#define MIcon(x)     QIcon(x)
#endif

namespace digital_clock {
namespace gui {

TrayControl::TrayControl(QWidget* parent) : QObject(parent)
{
  QMenu* tray_menu = new QMenu(parent);
  show_hide_action_ = tray_menu->addAction(MIcon(":/clock/images/clock.svg"), tr("&Hide Clock"),
                                           this, SLOT(ShowHideHandler()));
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
