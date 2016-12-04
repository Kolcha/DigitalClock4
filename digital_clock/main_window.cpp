/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "main_window.h"

#include <functional>

#include <QApplication>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QSettings>
#include <QDesktopWidget>
#include <QMenu>
#include <QDesktopServices>

#include "settings_storage.h"

#include "core/clock_settings.h"
#include "core/clock_state.h"
#include "core/skin_manager.h"
#include "core/updater.h"
#include "core/plugin_manager.h"

#include "gui/tray_control.h"
#include "gui/clock_widget.h"
#include "gui/settings_dialog.h"
#include "gui/about_dialog.h"


#define S_OPT_POSITION              "clock_position"


namespace digital_clock {

MainWindow::MainWindow(QWidget* parent) : QWidget(parent, Qt::Window)
{
  setWindowFlags(Qt::FramelessWindowHint);
#ifdef Q_OS_MACOS
  setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint);
#else
  setWindowFlags(windowFlags() | Qt::Tool | Qt::X11BypassWindowManagerHint);
#endif
  setAttribute(Qt::WA_TranslucentBackground);

  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::ShowContextMenu);

  config_backend_ = new SettingsStorage(this);
  app_config_ = new core::ClockSettings(config_backend_, config_backend_);
  state_ = new core::ClockState(config_backend_);

  skin_manager_ = new core::SkinManager(this);
  skin_manager_->ListSkins();
  skin_manager_->SetFallbackSkin("Electronic (default)");

  updater_ = new core::Updater(state_, this);
  connect(&timer_, &QTimer::timeout, updater_, &core::Updater::TimeoutHandler);

  tray_control_ = new gui::TrayControl(this);
  connect(tray_control_, &gui::TrayControl::VisibilityChanged, this, &MainWindow::setVisible);
  connect(tray_control_, &gui::TrayControl::ShowSettingsDlg, this, &MainWindow::EnsureVisible);
  connect(tray_control_, &gui::TrayControl::ShowSettingsDlg, this, &MainWindow::ShowSettingsDialog);
  connect(tray_control_, &gui::TrayControl::ShowAboutDlg, this, &MainWindow::ShowAboutDialog);
  connect(tray_control_, &gui::TrayControl::CheckForUpdates, updater_, &core::Updater::CheckForUpdates);
  connect(tray_control_, &gui::TrayControl::AppExit, this, &MainWindow::ShutdownPluginSystem);
  connect(tray_control_, &gui::TrayControl::AppExit, qApp, &QApplication::quit);

  clock_widget_ = new gui::ClockWidget(this);
  connect(clock_widget_, &gui::ClockWidget::changed, this, &MainWindow::Update);
  connect(clock_widget_, &gui::ClockWidget::SeparatorsChanged, skin_manager_, &core::SkinManager::SetSeparators);
  connect(&timer_, &QTimer::timeout, clock_widget_, &gui::ClockWidget::TimeoutHandler);
  connect(skin_manager_, &core::SkinManager::SkinLoaded, clock_widget_, &gui::ClockWidget::ApplySkin);

  ConnectTrayMessages();

  QGridLayout* main_layout = new QGridLayout(this);
  main_layout->addWidget(clock_widget_);
  main_layout->setMargin(2);
  setLayout(main_layout);
  adjustSize();

  last_visibility_ = true;

  InitPluginSystem();
  Reset();
  LoadState();

  timer_.setInterval(500);
  timer_.setSingleShot(false);
  timer_.start();
}

MainWindow::~MainWindow()
{
  timer_.stop();
  delete state_;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    drag_position_ = event->globalPos() - frameGeometry().topLeft();
    event->accept();
  }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
  if (event->buttons() & Qt::LeftButton) {
    move(event->globalPos() - drag_position_);
    event->accept();
  }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    QPoint last_pos = this->pos();
    if (cur_alignment_ == CAlignment::A_RIGHT) {
      last_pos.setX(this->frameGeometry().right());
    }
    state_->SetVariable(S_OPT_POSITION, last_pos, !clock_widget_->preview());
    event->accept();
  }
}

void MainWindow::paintEvent(QPaintEvent* /*event*/)
{
  QPainter p(this);
  p.setCompositionMode(QPainter::CompositionMode_Clear);
  p.fillRect(this->rect(), Qt::transparent);
  if (!testAttribute(Qt::WA_TranslucentBackground)) {
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.fillRect(this->rect(), bg_color_);
  }
}

void MainWindow::Reset()
{
  // widnow settings
  ApplyOption(OPT_OPACITY, app_config_->GetValue(OPT_OPACITY));
  ApplyOption(OPT_STAY_ON_TOP, app_config_->GetValue(OPT_STAY_ON_TOP));
  ApplyOption(OPT_TRANSP_FOR_INPUT, app_config_->GetValue(OPT_TRANSP_FOR_INPUT));
  ApplyOption(OPT_ALIGNMENT, app_config_->GetValue(OPT_ALIGNMENT));
  ApplyOption(OPT_BACKGROUND_COLOR, app_config_->GetValue(OPT_BACKGROUND_COLOR));
  ApplyOption(OPT_BACKGROUND_ENABLED, app_config_->GetValue(OPT_BACKGROUND_ENABLED));

  // load time format first to update separators where it required
  ApplyOption(OPT_TIME_FORMAT, app_config_->GetValue(OPT_TIME_FORMAT));
  clock_widget_->blockSignals(true);
  ApplyOption(OPT_SEPARATOR_FLASH, app_config_->GetValue(OPT_SEPARATOR_FLASH));
  ApplyOption(OPT_ZOOM, app_config_->GetValue(OPT_ZOOM));
  ApplyOption(OPT_COLOR, app_config_->GetValue(OPT_COLOR));
  ApplyOption(OPT_TEXTURE, app_config_->GetValue(OPT_TEXTURE));
  ApplyOption(OPT_TEXTURE_TYPE, app_config_->GetValue(OPT_TEXTURE_TYPE));
  ApplyOption(OPT_TEXTURE_PER_ELEMENT, app_config_->GetValue(OPT_TEXTURE_PER_ELEMENT));
  ApplyOption(OPT_TEXTURE_DRAW_MODE, app_config_->GetValue(OPT_TEXTURE_DRAW_MODE));
  ApplyOption(OPT_CUSTOMIZATION, app_config_->GetValue(OPT_CUSTOMIZATION));
  ApplyOption(OPT_SPACING, app_config_->GetValue(OPT_SPACING));
  ApplyOption(OPT_COLORIZE_COLOR, app_config_->GetValue(OPT_COLORIZE_COLOR));
  ApplyOption(OPT_COLORIZE_LEVEL, app_config_->GetValue(OPT_COLORIZE_LEVEL));

  ApplyOption(OPT_FONT, app_config_->GetValue(OPT_FONT));
  ApplyOption(OPT_SKIN_NAME, app_config_->GetValue(OPT_SKIN_NAME));
  clock_widget_->blockSignals(false);

  clock_widget_->TimeoutHandler();      // to apply changes

  // updater settings
  ApplyOption(OPT_USE_AUTOUPDATE, app_config_->GetValue(OPT_USE_AUTOUPDATE));
  ApplyOption(OPT_UPDATE_PERIOD, app_config_->GetValue(OPT_UPDATE_PERIOD));
  ApplyOption(OPT_CHECK_FOR_BETA, app_config_->GetValue(OPT_CHECK_FOR_BETA));

  // misc settings
  ApplyOption(OPT_CLOCK_URL_ENABLED, app_config_->GetValue(OPT_CLOCK_URL_ENABLED));
  ApplyOption(OPT_CLOCK_URL_STRING, app_config_->GetValue(OPT_CLOCK_URL_STRING));

  plugin_manager_->UnloadPlugins();
  plugin_manager_->LoadPlugins(app_config_->GetValue(OPT_PLUGINS).toStringList());
}

void MainWindow::ApplyOption(const Option opt, const QVariant& value)
{
  switch (opt) {
    case OPT_OPACITY:
      setWindowOpacity(value.toReal());
      break;

    case OPT_STAY_ON_TOP:
      SetWindowFlag(Qt::WindowStaysOnTopHint, value.toBool());
      break;

    case OPT_TRANSP_FOR_INPUT:
      SetWindowFlag(Qt::WindowTransparentForInput, value.toBool());
      break;

    case OPT_ALIGNMENT:
      cur_alignment_ = static_cast<CAlignment>(value.toInt());
      break;

    case OPT_BACKGROUND_ENABLED:
      this->setAttribute(Qt::WA_TranslucentBackground, !value.toBool());
      this->repaint();
      break;

    case OPT_BACKGROUND_COLOR:
      bg_color_ = value.value<QColor>();
      this->repaint();
      break;

    case OPT_SKIN_NAME:
      skin_manager_->LoadSkin(value.toString());
      break;

    case OPT_FONT:
      skin_manager_->SetFont(value.value<QFont>());
      break;

    case OPT_USE_AUTOUPDATE:
      updater_->SetAutoupdate(value.toBool());
      break;

    case OPT_UPDATE_PERIOD:
      updater_->SetUpdatePeriod(value.toInt());
      break;

    case OPT_CHECK_FOR_BETA:
      updater_->SetCheckForBeta(value.toBool());
      break;

    default:
      clock_widget_->ApplyOption(opt, value);
  }
}

void MainWindow::EnsureVisible()
{
  last_visibility_ = this->isVisible();
  if (!this->isVisible()) this->setVisible(true);
  tray_control_->GetShowHideAction()->setDisabled(true);
}

void MainWindow::RestoreVisibility()
{
  this->setVisible(last_visibility_);
  tray_control_->GetShowHideAction()->setEnabled(true);
}

void MainWindow::LoadState()
{
  QPoint last_pos = state_->GetVariable(S_OPT_POSITION, QPoint(50, 20)).toPoint();

  CAlignment last_align = static_cast<CAlignment>(app_config_->GetValue(OPT_ALIGNMENT).toInt());
  if (last_align == CAlignment::A_RIGHT) {
    last_pos.setX(last_pos.x() - this->width());
  }
  cur_alignment_ = last_align;
  this->move(last_pos);
  CorrectPosition();
}

void MainWindow::SaveState()
{
  QPoint last_pos = this->pos();
  if (cur_alignment_ == CAlignment::A_RIGHT) {
    last_pos.setX(this->frameGeometry().right());
  }
  state_->SetVariable(S_OPT_POSITION, last_pos);
}

void MainWindow::ShowSettingsDialog()
{
  static QPointer<gui::SettingsDialog> dlg;
  if (!dlg) {
    dlg = new gui::SettingsDialog(app_config_, state_);
    // main signals/slots: change options, apply and reset
    connect(dlg.data(), &gui::SettingsDialog::OptionChanged, this, &MainWindow::ApplyOption);
    connect(dlg.data(), &gui::SettingsDialog::OptionChanged, app_config_, &core::ClockSettings::SetValue);
    connect(dlg.data(), &gui::SettingsDialog::accepted, app_config_, &core::ClockSettings::Accept);
    connect(app_config_, &core::ClockSettings::accepted, config_backend_, &SettingsStorage::Accept);
    connect(app_config_, &core::ClockSettings::accepted, this, &MainWindow::SaveState);
    connect(dlg.data(), &gui::SettingsDialog::rejected, config_backend_, &SettingsStorage::Reject);
    connect(dlg.data(), &gui::SettingsDialog::ResetSettings, config_backend_, &SettingsStorage::Reset);
    connect(app_config_, &core::ClockSettings::rejected, this, &MainWindow::Reset);
    connect(app_config_, &core::ClockSettings::rejected, this, &MainWindow::LoadState);
    // restore clock visibility
    connect(dlg.data(), &gui::SettingsDialog::accepted, this, &MainWindow::RestoreVisibility);
    connect(app_config_, &core::ClockSettings::rejected, this, &MainWindow::RestoreVisibility);
    // export/import
    connect(dlg.data(), &gui::SettingsDialog::ExportSettings, config_backend_, &SettingsStorage::Export);
    connect(dlg.data(), &gui::SettingsDialog::ImportSettings, config_backend_, &SettingsStorage::Import);
    // check for updates
    connect(dlg.data(), &gui::SettingsDialog::CheckForUpdates, updater_, &core::Updater::CheckForUpdates);
    // skins list
    connect(skin_manager_, &core::SkinManager::SearchFinished, dlg.data(), &gui::SettingsDialog::SetSkinList);
    skin_manager_->ListSkins();
    // 'preview mode' support
    clock_widget_->EnablePreviewMode();
    connect(dlg.data(), &gui::SettingsDialog::destroyed, clock_widget_, &gui::ClockWidget::DisablePreviewMode);

    // plugins engine
    connect(dlg.data(), &gui::SettingsDialog::OptionChanged, plugin_manager_, &core::PluginManager::UpdateSettings);
    // plugins list
    connect(plugin_manager_, &core::PluginManager::SearchFinished, dlg.data(), &gui::SettingsDialog::SetPluginsList);
    plugin_manager_->ListAvailable();
    // enable/disable plugin, configure plugin
    connect(dlg.data(), &gui::SettingsDialog::PluginStateChanged, plugin_manager_, &core::PluginManager::EnablePlugin);
    connect(dlg.data(), &gui::SettingsDialog::PluginConfigureRequest, plugin_manager_, &core::PluginManager::ConfigurePlugin);

    dlg->show();
  }
  dlg->raise();
  dlg->activateWindow();
}

void MainWindow::ShowAboutDialog()
{
  static QPointer<gui::AboutDialog> dlg;
  if (!dlg) {
    dlg = new gui::AboutDialog();
    dlg->show();
  }
  dlg->raise();
  dlg->activateWindow();
}

void MainWindow::ShowContextMenu(const QPoint& p)
{
  tray_control_->GetTrayIcon()->contextMenu()->exec(this->mapToParent(p));
}

void MainWindow::Update()
{
  if (cur_alignment_ == CAlignment::A_RIGHT) {
    int old_width = this->frameGeometry().width();
    this->adjustSize();
    int new_width = this->frameGeometry().width();
    QPoint cur_pos = this->pos();
    cur_pos.setX(cur_pos.x() + old_width - new_width);
    this->move(cur_pos);
  } else {
    Q_ASSERT(cur_alignment_ == CAlignment::A_LEFT);
    this->adjustSize();
  }
#ifdef Q_OS_WIN
  // always on top problem workaround
  // https://forum.qt.io/topic/28739/flags-windows-7-window-always-on-top-including-the-win7-taskbar-custom-error/4
  if (app_config_->GetValue(OPT_STAY_ON_TOP).toBool() && !this->isActiveWindow()) {
    this->raise();
  }
#endif
  CorrectPosition();
}

void MainWindow::InitPluginSystem()
{
  plugin_manager_ = new core::PluginManager(this);
  plugin_manager_->ListAvailable();

  core::TPluginData plugin_data;
  plugin_data.settings = app_config_;
  plugin_data.tray = tray_control_->GetTrayIcon();
  plugin_data.window = clock_widget_;

  plugin_manager_->SetInitData(plugin_data);
}

void MainWindow::ShutdownPluginSystem()
{
  plugin_manager_->UnloadPlugins();
}

void MainWindow::ConnectTrayMessages()
{
  // updater messages
  connect(updater_, &core::Updater::ErrorMessage, [this] (const QString& msg) {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Update").arg(qApp->applicationName()),
          tr("Update error. %1").arg(msg),
          QSystemTrayIcon::Critical);
  });

  connect(updater_, &core::Updater::UpToDate, [this] () {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Update").arg(qApp->applicationName()),
          tr("You already have latest version (%1).").arg(qApp->applicationVersion()),
          QSystemTrayIcon::Information);
  });

  connect(updater_, &core::Updater::NewVersion, [this] (const QString& version, const QString& link) {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Update").arg(qApp->applicationName()),
          tr("Update available (%1). Click this message to download.").arg(version),
          QSystemTrayIcon::Warning);
    connect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked,
            [=] () { QDesktopServices::openUrl(link); });
  });

  // skin_manager messages
  connect(skin_manager_, &core::SkinManager::ErrorMessage, [this] (const QString& msg) {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Error").arg(qApp->applicationName()), msg, QSystemTrayIcon::Warning);
  });
}

void MainWindow::SetWindowFlag(Qt::WindowFlags flag, bool set)
{
  QWidget* aw = QApplication::activeWindow();
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
  if (aw) aw->activateWindow();
}

void MainWindow::CorrectPosition()
{
#ifndef Q_OS_MACOS  // workaround for https://sourceforge.net/p/digitalclock4/tickets/7/
  QPoint curr_pos = this->pos();
  QDesktopWidget* desktop = QApplication::desktop();
  curr_pos.setX(std::max(curr_pos.x(), desktop->geometry().left()));
  curr_pos.setX(std::min(curr_pos.x(), desktop->geometry().right() - this->width()));
  curr_pos.setY(std::max(curr_pos.y(), desktop->geometry().top()));
  curr_pos.setY(std::min(curr_pos.y(), desktop->geometry().bottom() - this->height()));
  if (curr_pos != this->pos()) this->move(curr_pos);
#endif
}

} // namespace digital_clock
