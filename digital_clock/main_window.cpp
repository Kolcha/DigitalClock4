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
#include "core/skin_manager.h"
#include "core/updater.h"
#include "core/plugin_manager.h"

#include "gui/tray_control.h"
#include "gui/clock_widget.h"
#include "gui/settings_dialog.h"
#include "gui/about_dialog.h"


#define S_OPT_POSITION              "state/clock_position"


namespace digital_clock {

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
  setWindowFlags(Qt::FramelessWindowHint);
#ifdef Q_OS_OSX
  setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint);
#else
  setWindowFlags(windowFlags() | Qt::Tool);
#endif
  setAttribute(Qt::WA_TranslucentBackground);

  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::ShowContextMenu);

  config_backend_ = new SettingsStorage(this);
  app_config_ = new core::ClockSettings(config_backend_, config_backend_);

  skin_manager_ = new core::SkinManager(this);
  skin_manager_->ListSkins();
  skin_manager_->SetFallbackSkin("Electronic (default)");

  updater_ = new core::Updater(this);
  connect(&timer_, &QTimer::timeout, updater_, &core::Updater::TimeoutHandler);

  tray_control_ = new gui::TrayControl(this);
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
  setLayout(main_layout);
  adjustSize();



  QSettings state;
  QPoint last_pos = state.value(S_OPT_POSITION, QPoint(50, 20)).toPoint();

  CAlignment last_align = static_cast<CAlignment>(app_config_->GetValue(OPT_ALIGNMENT).toInt());
  if (last_align == CAlignment::A_RIGHT) {
    last_pos.setX(last_pos.x() - this->width());
  }
  cur_alignment_ = last_align;
  this->move(last_pos);



  InitPluginSystem();
  Reset();

  timer_.setInterval(500);
  timer_.setSingleShot(false);
  timer_.start();
}

MainWindow::~MainWindow()
{
  timer_.stop();
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
    QSettings state;
    state.setValue(S_OPT_POSITION, last_pos);
    event->accept();
  }
}

void MainWindow::paintEvent(QPaintEvent* /*event*/)
{
  QPainter p(this);
  p.setCompositionMode(QPainter::CompositionMode_Clear);
  p.fillRect(this->rect(), Qt::transparent);
}

void MainWindow::Reset()
{
  // widnow settings
  ApplyOption(OPT_OPACITY, app_config_->GetValue(OPT_OPACITY));
  ApplyOption(OPT_STAY_ON_TOP, app_config_->GetValue(OPT_STAY_ON_TOP));
  ApplyOption(OPT_TRANSP_FOR_INPUT, app_config_->GetValue(OPT_TRANSP_FOR_INPUT));
  ApplyOption(OPT_ALIGNMENT, app_config_->GetValue(OPT_ALIGNMENT));

  clock_widget_->blockSignals(true);
  ApplyOption(OPT_SEPARATOR_FLASH, app_config_->GetValue(OPT_SEPARATOR_FLASH));
  ApplyOption(OPT_TIME_FORMAT, app_config_->GetValue(OPT_TIME_FORMAT));
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
    {
      cur_alignment_ = static_cast<CAlignment>(value.toInt());
      QPoint cur_pos = this->pos();
      switch (cur_alignment_) {
        case CAlignment::A_LEFT:
        {
          if (cur_pos.x() < 0) {
            cur_pos.setX(0);
            this->move(cur_pos);
          }
          break;
        }

        case CAlignment::A_RIGHT:
        {
          cur_pos = this->frameGeometry().topRight();
          QDesktopWidget desktop;
          if (cur_pos.x() > desktop.screen()->width()) {
            cur_pos.setX(desktop.screen()->width());
            this->move(cur_pos.x() - this->width(), cur_pos.y());
          }
          break;
        }

        default:
          Q_ASSERT(false);
      }
      QSettings state;
      state.setValue(S_OPT_POSITION, cur_pos);
      break;
    }

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

void MainWindow::ShowSettingsDialog()
{
  static QPointer<gui::SettingsDialog> dlg;
  if (!dlg) {
    dlg = new gui::SettingsDialog(app_config_);
    // main signals/slots: change options, apply and reset
    connect(dlg.data(), &gui::SettingsDialog::OptionChanged, this, &MainWindow::ApplyOption);
    connect(dlg.data(), &gui::SettingsDialog::OptionChanged, app_config_, &core::ClockSettings::SetValue);
    connect(dlg.data(), &gui::SettingsDialog::accepted, app_config_, &core::ClockSettings::Save);
    connect(app_config_, &core::ClockSettings::saved, config_backend_, &SettingsStorage::Save);
    connect(dlg.data(), &gui::SettingsDialog::rejected, config_backend_, &SettingsStorage::Load);
    connect(dlg.data(), &gui::SettingsDialog::ResetSettings, config_backend_, &SettingsStorage::Reset);
    connect(app_config_, &core::ClockSettings::reloaded, this, &MainWindow::Reset);
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
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}

} // namespace digital_clock
