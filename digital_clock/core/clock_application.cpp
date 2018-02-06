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

#include "clock_application.h"

#include <functional>

#include <QAction>
#include <QUrl>
#include <QDesktopServices>
#include <QApplication>

#include "settings_storage.h"

#include "core/clock_settings.h"
#include "core/clock_state.h"
#include "core/plugin_manager.h"
#include "core/skin_manager.h"
#include "core/updater.h"

#include "gui/clock_window.h"
#include "gui/clock_widget.h"
#include "gui/context_menu.h"
#include "gui/about_dialog.h"
#include "gui/settings_dialog.h"
#include "gui/tray_control.h"

namespace digital_clock {
namespace core {

ClockApplication::ClockApplication(ClockSettings* config, QObject* parent) :
  QObject(parent),
  config_backend_(config->GetBackend()),
  app_config_(config)
{
  state_ = new core::ClockState(config_backend_);

  CreateWindows();

  skin_manager_ = new core::SkinManager(this);
  skin_manager_->ListSkins();
  skin_manager_->SetFallbackSkin("Electronic (default)");

  updater_ = new core::Updater(state_, this);
  connect(&timer_, &QTimer::timeout, updater_, &core::Updater::TimeoutHandler);

  tray_control_ = new gui::TrayControl(this);
  for (auto w : clock_windows_) {
    connect(tray_control_, &gui::TrayControl::VisibilityChanged, w, &gui::ClockWindow::ChangeVisibility);
    connect(tray_control_, &gui::TrayControl::PositionChanged, w, &gui::ClockWindow::MoveWindow);
  }
  connect(tray_control_, &gui::TrayControl::ShowSettingsDlg, this, &ClockApplication::ShowSettingsDialog);
  connect(tray_control_, &gui::TrayControl::ShowAboutDlg, this, &ClockApplication::ShowAboutDialog);
  connect(tray_control_, &gui::TrayControl::CheckForUpdates, updater_, &core::Updater::CheckForUpdates);
  connect(tray_control_, &gui::TrayControl::AppExit, qApp, &QApplication::quit);

  for (auto w : clock_windows_) {
    connect(&timer_, &QTimer::timeout, w, &gui::ClockWindow::TimeoutHandler);
    connect(skin_manager_, &SkinManager::SkinLoaded, w, &gui::ClockWindow::ApplySkin);
    connect(w->clockWidget(), &gui::ClockWidget::SeparatorsChanged, skin_manager_, &SkinManager::SetSeparators);
    // window context menu
    connect(w->contextMenu(), &gui::ContextMenu::VisibilityChanged, this, &ClockApplication::UpdateVisibilityAction);
    connect(w->contextMenu(), &gui::ContextMenu::ShowSettingsDlg, this, &ClockApplication::ShowSettingsDialog);
    connect(w->contextMenu(), &gui::ContextMenu::ShowAboutDlg, this, &ClockApplication::ShowAboutDialog);
    connect(w->contextMenu(), &gui::ContextMenu::CheckForUpdates, updater_, &Updater::CheckForUpdates);
    connect(w->contextMenu(), &gui::ContextMenu::AppExit, qApp, &QApplication::quit);
  }

  ConnectTrayMessages();

  InitPluginSystem();
  Reset();
  for (auto w : clock_windows_) w->LoadState();
  UpdateVisibilityAction();

  timer_.setInterval(500);
  timer_.setSingleShot(false);
  timer_.start();
}

ClockApplication::~ClockApplication()
{
  ShutdownPluginSystem();
  timer_.stop();
  for (auto w : clock_windows_) delete w;
  delete state_;
  delete tray_control_;
}

void ClockApplication::UpdateVisibilityAction()
{
  bool checked = false;
  for (auto w : clock_windows_) {
    checked = checked || w->isVisible();
    if (checked) break;
  }
  tray_control_->GetShowHideAction()->setChecked(checked);
}

void ClockApplication::Reset()
{
  // widnow settings
  ApplyOption(OPT_OPACITY, app_config_->GetValue(OPT_OPACITY));
  ApplyOption(OPT_FULLSCREEN_DETECT, app_config_->GetValue(OPT_FULLSCREEN_DETECT));
  ApplyOption(OPT_STAY_ON_TOP, app_config_->GetValue(OPT_STAY_ON_TOP));
  ApplyOption(OPT_TRANSP_FOR_INPUT, app_config_->GetValue(OPT_TRANSP_FOR_INPUT));
  ApplyOption(OPT_ALIGNMENT, app_config_->GetValue(OPT_ALIGNMENT));
  ApplyOption(OPT_BACKGROUND_COLOR, app_config_->GetValue(OPT_BACKGROUND_COLOR));
  ApplyOption(OPT_BACKGROUND_ENABLED, app_config_->GetValue(OPT_BACKGROUND_ENABLED));
  ApplyOption(OPT_SHOW_WINDOW_BORDER, app_config_->GetValue(OPT_SHOW_WINDOW_BORDER));
  ApplyOption(OPT_SNAP_TO_EDGES, app_config_->GetValue(OPT_SNAP_TO_EDGES));
  ApplyOption(OPT_SNAP_THRESHOLD, app_config_->GetValue(OPT_SNAP_THRESHOLD));

  // load time format first to update separators where it required
  ApplyOption(OPT_TIME_FORMAT, app_config_->GetValue(OPT_TIME_FORMAT));
  for (auto w : clock_windows_) w->blockSignals(true);
  ApplyOption(OPT_SEPARATOR_FLASH, app_config_->GetValue(OPT_SEPARATOR_FLASH));
  ApplyOption(OPT_TIME_ZONE, app_config_->GetValue(OPT_TIME_ZONE));
  ApplyOption(OPT_DISPLAY_LOCAL_TIME, app_config_->GetValue(OPT_DISPLAY_LOCAL_TIME));
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
  for (auto w : clock_windows_) w->blockSignals(false);

  for (auto w : clock_windows_) w->TimeoutHandler();      // to apply changes

  // updater settings
  ApplyOption(OPT_USE_AUTOUPDATE, app_config_->GetValue(OPT_USE_AUTOUPDATE));
  ApplyOption(OPT_UPDATE_PERIOD, app_config_->GetValue(OPT_UPDATE_PERIOD));
  ApplyOption(OPT_CHECK_FOR_BETA, app_config_->GetValue(OPT_CHECK_FOR_BETA));

  // misc settings
  ApplyOption(OPT_CLOCK_URL_ENABLED, app_config_->GetValue(OPT_CLOCK_URL_ENABLED));
  ApplyOption(OPT_CLOCK_URL_STRING, app_config_->GetValue(OPT_CLOCK_URL_STRING));
  ApplyOption(OPT_SHOW_HIDE_ENABLED, app_config_->GetValue(OPT_SHOW_HIDE_ENABLED));
  ApplyOption(OPT_EXPORT_STATE, app_config_->GetValue(OPT_EXPORT_STATE));
  ApplyOption(OPT_KEEP_ALWAYS_VISIBLE, app_config_->GetValue(OPT_KEEP_ALWAYS_VISIBLE));

  plugin_manager_->UnloadPlugins();
  plugin_manager_->LoadPlugins(app_config_->GetValue(OPT_PLUGINS).toStringList());
}

void ClockApplication::ApplyOption(const Option opt, const QVariant& value)
{
  switch (opt) {
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

    case OPT_SHOW_HIDE_ENABLED:
      tray_control_->GetShowHideAction()->setVisible(value.toBool());
      // fallthrough

    default:
      for (auto w : clock_windows_) w->ApplyOption(opt, value);
  }
}

void ClockApplication::ShowSettingsDialog()
{
  clock_windows_.first()->activateWindow();
  clock_windows_.first()->raise();
  static QPointer<gui::SettingsDialog> dlg;
  if (!dlg) {
    dlg = new gui::SettingsDialog(app_config_, state_);
    // main signals/slots: change options, apply and reset
    connect(dlg.data(), &gui::SettingsDialog::OptionChanged, this, &ClockApplication::ApplyOption);
    connect(dlg.data(), &gui::SettingsDialog::OptionChanged, app_config_, &core::ClockSettings::SetValue);
    connect(dlg.data(), &gui::SettingsDialog::accepted, app_config_, &core::ClockSettings::Accept);
    connect(app_config_, &core::ClockSettings::accepted, config_backend_, &SettingsStorage::Accept);
    connect(app_config_, &core::ClockSettings::accepted, state_, &core::ClockState::Accept);
    connect(dlg.data(), &gui::SettingsDialog::rejected, config_backend_, &SettingsStorage::Reject);
    connect(dlg.data(), &gui::SettingsDialog::ResetSettings, config_backend_, &SettingsStorage::Reset);
    connect(app_config_, &core::ClockSettings::rejected, this, &ClockApplication::Reset);
    connect(app_config_, &core::ClockSettings::rejected, state_, &core::ClockState::Reject);
    // TODO: connect common (not related to SettingsDialog) signal/slots in constructor
    // export/import
    connect(dlg.data(), &gui::SettingsDialog::ExportSettings, config_backend_, &SettingsStorage::Export);
    connect(dlg.data(), &gui::SettingsDialog::ImportSettings, config_backend_, &SettingsStorage::Import);
    // check for updates
    connect(dlg.data(), &gui::SettingsDialog::CheckForUpdates, updater_, &core::Updater::CheckForUpdates);
    // skins list
    connect(skin_manager_, &core::SkinManager::SearchFinished, dlg.data(), &gui::SettingsDialog::SetSkinList);
    skin_manager_->ListSkins();
    // 'preview mode' support
    tray_control_->GetShowHideAction()->setEnabled(false);
    for (auto w : clock_windows_) {
      w->EnablePreviewMode();
      w->EnsureVisible();
      connect(dlg.data(), &gui::SettingsDialog::destroyed, w, &gui::ClockWindow::RestoreVisibility);
      connect(dlg.data(), &gui::SettingsDialog::destroyed, w, &gui::ClockWindow::DisablePreviewMode);
    }
    connect(dlg.data(), &gui::SettingsDialog::destroyed, [this] () { tray_control_->GetShowHideAction()->setEnabled(true); });
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

void ClockApplication::ShowAboutDialog()
{
  static QPointer<gui::AboutDialog> dlg;
  if (!dlg) {
    dlg = new gui::AboutDialog();
    dlg->show();
  }
  dlg->raise();
  dlg->activateWindow();
}

void ClockApplication::InitPluginSystem()
{
  plugin_manager_ = new core::PluginManager(this);
  plugin_manager_->ListAvailable();

  core::TPluginData plugin_data;
  plugin_data.settings = app_config_;
  plugin_data.tray = tray_control_->GetTrayIcon();
  for (auto w : clock_windows_)
    plugin_data.windows.append(w);

  plugin_manager_->SetInitData(plugin_data);
}

void ClockApplication::ShutdownPluginSystem()
{
  plugin_manager_->UnloadPlugins();
}

void ClockApplication::CreateWindows()
{
  int n = app_config_->GetValue(OPT_SHOW_ON_ALL_MONITORS).toBool() ? QApplication::screens().size() : 1;
  for (int i = 0; i < n; ++i) {
    clock_windows_.append(new gui::ClockWindow(app_config_, i + 1));
  }
}

void ClockApplication::ConnectTrayMessages()
{
  // updater messages
  connect(updater_, &core::Updater::ErrorMessage, [this] (const QString& msg) {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    // *INDENT-OFF*
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Update").arg(qApp->applicationName()),
          tr("Update error. %1").arg(msg),
          QSystemTrayIcon::Critical);
    // *INDENT-ON*
  });

  connect(updater_, &core::Updater::UpToDate, [this] () {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    // *INDENT-OFF*
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Update").arg(qApp->applicationName()),
          tr("You already have latest version (%1).").arg(qApp->applicationVersion()),
          QSystemTrayIcon::Information);
    // *INDENT-ON*
  });

  connect(updater_, &core::Updater::NewVersion, [this] (const QString& version, const QString& link) {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    // *INDENT-OFF*
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Update").arg(qApp->applicationName()),
          tr("Update available (%1). Click this message to download.").arg(version),
          QSystemTrayIcon::Warning);
    connect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked,
            [=] () { QDesktopServices::openUrl(link); });
    // *INDENT-ON*
  });

  // skin_manager messages
  connect(skin_manager_, &core::SkinManager::ErrorMessage, [this] (const QString& msg) {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    // *INDENT-OFF*
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Error").arg(qApp->applicationName()), msg, QSystemTrayIcon::Warning);
    // *INDENT-ON*
  });
}

} // namespace core
} // namespace digital_clock
