#include <QGridLayout>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QDesktopServices>
#include "settings_dialog.h"
#include "about_dialog.h"
#include "main_window.h"

#define OPT_POSITION_KEY          "clock/position"

using skin_draw::SkinDrawer;

namespace digital_clock {

MainWindow::MainWindow(QWidget* parent)
  : QWidget(parent) {
  // create objects
  plugin_manager_ = new PluginManager(this);
  skin_manager_ = new SkinManager(this);
  drawer_ = new SkinDrawer(this);
  d_clock_ = new DigitalClock(this);
  tray_control_ = new TrayControl(this);
  updater_ = new Updater(this);
  settings_ = new ClockSettings(this);

  // add clock widget
  QGridLayout* main_layout = new QGridLayout(this);
  main_layout->addWidget(d_clock_);
  setLayout(main_layout);

  // set window properties
  setWindowTitle("Clock");
  setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
  setAttribute(Qt::WA_TranslucentBackground);
  setContextMenuPolicy(Qt::CustomContextMenu);
}

MainWindow::~MainWindow() {
  plugin_manager_->UnloadPlugins();
}

void MainWindow::Init() {
  // connect signals
  ConnectAll();
  // init skin manager
  skin_manager_->AddSkinDir(QDir(":/clock/default_skin"));
  skin_manager_->AddSkinDir(QDir(QCoreApplication::applicationDirPath() + "/skins"));
  skin_manager_->ListSkins();
  // init plugin manager
  plugin_manager_->AddPluginsDir(QDir(QCoreApplication::applicationDirPath() + "/plugins"));
  plugin_manager_->ListAvailable();
  TPluginData plugin_data;
  plugin_data.clock = d_clock_;
  plugin_data.settings = settings_;
  plugin_data.tray = tray_control_->GetTrayIcon();
  plugin_data.window = this;
  plugin_manager_->SetInitData(plugin_data);

  // load application settings
  settings_->Load();
  d_clock_->SetSeparatorFlash(settings_->GetOption(OPT_SEPARATOR_FLASH).toBool());
  d_clock_->SetTimeFormat(settings_->GetOption(OPT_TIME_FORMAT).toString());
  skin_manager_->SetFont(settings_->GetOption(OPT_FONT).value<QFont>());
  skin_manager_->LoadSkin(settings_->GetOption(OPT_SKIN_NAME).toString());
  drawer_->SetZoom(settings_->GetOption(OPT_ZOOM).toReal());
  drawer_->SetColor(settings_->GetOption(OPT_COLOR).value<QColor>());
  drawer_->SetTexture(settings_->GetOption(OPT_TEXTURE).toString());
  drawer_->SetTexturePerElement(settings_->GetOption(OPT_TEXTURE_PER_ELEMENT).toBool());
  drawer_->SetTextureDrawMode((SkinDrawer::DrawMode)
                              (settings_->GetOption(OPT_TEXTURE_DRAW_MODE).toInt()));
  drawer_->SetCustomizationType((SkinDrawer::CustomizationType)
                                (settings_->GetOption(OPT_CUSTOMIZATION).toInt()));
  drawer_->SetString("88:88");

  active_plugins_ = settings_->GetOption(OPT_PLUGINS).toStringList();

  QTimer* timer = new QTimer();
  timer->setInterval(1000);
  timer->setSingleShot(true);
  connect(timer, &QTimer::timeout, [=] () { plugin_manager_->LoadPlugins(active_plugins_); });
  connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
  timer->start();

  updater_->SetCheckForBeta(settings_->GetOption(OPT_CHECK_FOR_BETA).toBool());
  updater_->SetAutoupdate(settings_->GetOption(OPT_USE_AUTOUPDATE).toBool());
  updater_->SetUpdatePeriod(settings_->GetOption(OPT_UPDATE_PERIOD).value<qint64>());

  // apply custom window flags if needed
  Qt::WindowFlags flags = windowFlags();
  if (settings_->GetOption(OPT_STAY_ON_TOP).toBool()) flags |= Qt::WindowStaysOnTopHint;
  if (settings_->GetOption(OPT_TRANSP_FOR_INPUT).toBool()) flags |= Qt::WindowTransparentForInput;
  setWindowFlags(flags);
  // load last position
  QSettings settings;
  move(settings.value(OPT_POSITION_KEY, QPoint(50, 20)).toPoint());
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
  if (event->buttons() & Qt::LeftButton) {
    move(event->globalPos() - drag_position_);
    event->accept();
  }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    drag_position_ = event->globalPos() - frameGeometry().topLeft();
    event->accept();
  }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    QSettings settings;
    settings.setValue(OPT_POSITION_KEY, pos());
    event->accept();
  }
}

void MainWindow::showEvent(QShowEvent* event) {
  setWindowOpacity(settings_->GetOption(OPT_OPACITY).toReal());
  event->accept();
}

void MainWindow::SettingsListener(Options opt, const QVariant& value) {
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

    case OPT_SEPARATOR_FLASH:
      d_clock_->SetSeparatorFlash(value.toBool());
      break;

    case OPT_TIME_FORMAT:
      d_clock_->SetTimeFormat(value.toString());
      break;

    case OPT_SKIN_NAME:
      skin_manager_->LoadSkin(value.toString());
      break;

    case OPT_FONT:
      skin_manager_->SetFont(value.value<QFont>());
      break;

    case OPT_ZOOM:
      drawer_->SetZoom(value.toReal());
      break;

    case OPT_COLOR:
      drawer_->SetColor(value.value<QColor>());
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(value.toString());
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((SkinDrawer::DrawMode)value.toInt());
      break;

    case OPT_CUSTOMIZATION:
      drawer_->SetCustomizationType((SkinDrawer::CustomizationType)value.toInt());
      break;

    case OPT_PLUGINS:
    {
      QStringList new_plugins = value.toStringList();
      for (auto& plugin : active_plugins_) {
        if (!new_plugins.contains(plugin)) plugin_manager_->EnablePlugin(plugin, false);
      }
      for (auto& plugin : new_plugins) {
        if (!active_plugins_.contains(plugin)) plugin_manager_->EnablePlugin(plugin, true);
      }
      active_plugins_ = new_plugins;
      break;
    }

    case OPT_USE_AUTOUPDATE:
      updater_->SetAutoupdate(value.toBool());
      break;

    case OPT_UPDATE_PERIOD:
      updater_->SetUpdatePeriod(value.value<qint64>());
      break;

    case OPT_CHECK_FOR_BETA:
      updater_->SetCheckForBeta(value.toBool());
      break;
  }
}

void MainWindow::ShowSettingsDialog() {
  if (settings_dlg_) {
    settings_dlg_->activateWindow();
  } else {
    // create settings dialog and connect all need signals
    // (settings dialog will be deleted automatically)
    settings_dlg_ = new SettingsDialog();
    connect(skin_manager_, SIGNAL(SearchFinished(QStringList)),
            settings_dlg_, SLOT(SetSkinList(QStringList)));
    skin_manager_->ListSkins();
    connect(skin_manager_, SIGNAL(SkinInfoLoaded(ClockBaseSkin::TSkinInfo)),
            settings_dlg_, SLOT(DisplaySkinInfo(ClockBaseSkin::TSkinInfo)));
    connect(plugin_manager_, SIGNAL(SearchFinished(QList<QPair<TPluginInfo,bool> >)),
            settings_dlg_, SLOT(SetPluginsList(QList<QPair<TPluginInfo,bool> >)));
    plugin_manager_->ListAvailable();

    // reload settings to emit signals needed to init settings dialog controls
    // with current values
    connect(settings_, SIGNAL(OptionChanged(Options,QVariant)),
            settings_dlg_, SLOT(SettingsListener(Options,QVariant)));
    settings_->TrackChanges(true);
    settings_->Load();
    settings_dlg_->show();
    // disable settings listener for settings dialog
    disconnect(settings_, SIGNAL(OptionChanged(Options,QVariant)),
               settings_dlg_, SLOT(SettingsListener(Options,QVariant)));
    // connect main logic signals: change/save/discard settings
    connect(settings_dlg_, SIGNAL(OptionChanged(Options,QVariant)),
            settings_, SLOT(SetOption(Options,QVariant)));
    connect(settings_dlg_, SIGNAL(PluginConfigureRequest(QString)),
            plugin_manager_, SLOT(ConfigurePlugin(QString)));
    connect(settings_dlg_, SIGNAL(accepted()), settings_, SLOT(Save()));
    connect(settings_dlg_, SIGNAL(rejected()), settings_, SLOT(Load()));

    connect(settings_dlg_, SIGNAL(destroyed()), this, SLOT(EndSettingsEdit()));
    drawer_->SetPreviewMode(true);
  }
}

void MainWindow::EndSettingsEdit() {
  drawer_->SetPreviewMode(false);
  settings_->TrackChanges(false);
}

void MainWindow::ShowAboutDialog() {
  if (about_dlg_) {
    about_dlg_->activateWindow();
  } else {
    about_dlg_ = new AboutDialog(this);
    about_dlg_->show();
  }
}

void MainWindow::DisplayMenu(const QPoint& pos) {
  tray_control_->GetTrayIcon()->contextMenu()->exec(mapToParent(pos));
}

void MainWindow::ConnectAll() {
  connect(settings_, SIGNAL(OptionChanged(Options,QVariant)),
          this, SLOT(SettingsListener(Options,QVariant)));
  connect(skin_manager_, &SkinManager::SkinLoaded, drawer_, &SkinDrawer::ApplySkin);
  connect(drawer_, SIGNAL(DrawingFinished(QImage)), d_clock_, SLOT(DrawImage(QImage)));
  connect(d_clock_, SIGNAL(ImageNeeded(QString)), drawer_, SLOT(SetString(QString)));
  connect(d_clock_, SIGNAL(SeparatorsChanged(QString)),
          skin_manager_, SLOT(SetSeparators(QString)));
  connect(tray_control_, SIGNAL(ShowSettingsDlg()), this, SLOT(ShowSettingsDialog()));
  connect(tray_control_, SIGNAL(ShowAboutDlg()), this, SLOT(ShowAboutDialog()));
  connect(tray_control_, &TrayControl::CheckForUpdates, [=](){ updater_->CheckForUpdates(true); });
  connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(DisplayMenu(QPoint)));

  connect(updater_, &Updater::ErrorMessage, [=] (const QString& msg) {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Update").arg(QCoreApplication::applicationName()),
          tr("Update error. %1").arg(msg),
          QSystemTrayIcon::Critical);
  });

  connect(updater_, &Updater::UpToDate, [=] () {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Update").arg(QCoreApplication::applicationName()),
          tr("You already have latest version (%1).").arg(QCoreApplication::applicationVersion()),
          QSystemTrayIcon::Information);
  });

  connect(updater_, &Updater::NewVersion, [=] (const QString& version, const QString& link) {
    disconnect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked, 0, 0);
    tray_control_->GetTrayIcon()->showMessage(
          tr("%1 Update").arg(QCoreApplication::applicationName()),
          tr("Update available (%1). Click this message to download.").arg(version),
          QSystemTrayIcon::Warning);
    connect(tray_control_->GetTrayIcon(), &QSystemTrayIcon::messageClicked,
            [=] () { QDesktopServices::openUrl(link); });
  });
  connect(d_clock_, SIGNAL(ImageNeeded(QString)), updater_, SLOT(TimeoutHandler()));
}

void MainWindow::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}

} // namespace digital_clock
