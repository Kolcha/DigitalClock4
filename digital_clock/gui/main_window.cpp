#include <QGridLayout>
#include <QMouseEvent>
#include <QCoreApplication>
#include "settings_dialog.h"
#include "about_dialog.h"
#include "main_window.h"

#define OPT_POSITION_KEY          "clock/position"

MainWindow::MainWindow(QWidget* parent)
  : QWidget(parent) {
  // create objects
  settings_ = new ClockSettings(this);
  plugin_manager_ = new PluginManager(this);
  skin_manager_ = new SkinManager(this);
  drawer_ = new SkinDrawer(this);
  d_clock_ = new DigitalClock(this);
  tray_control_ = new TrayControl(this);

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

void MainWindow::Init() {
  // connect signals
  ConnectAll();
  // init skin manager
  skin_manager_->AddSkinDir(QDir(":/default_skin"));
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
  if (settings_->GetOption(OPT_USE_SKIN).toBool())
    skin_manager_->FindSkin(settings_->GetOption(OPT_SKIN_NAME).toString());
  if (settings_->GetOption(OPT_USE_FONT).toBool())
    drawer_->LoadSkin(settings_->GetOption(OPT_FONT).value<QFont>());
  drawer_->SetZoom(settings_->GetOption(OPT_ZOOM).toReal());
  drawer_->SetColor(settings_->GetOption(OPT_COLOR).value<QColor>());
  drawer_->SetTexture(settings_->GetOption(OPT_TEXTURE).toString());
  drawer_->SetTexturePerElement(settings_->GetOption(OPT_TEXTURE_PER_ELEMENT).toBool());
  drawer_->SetTextureDrawMode((SkinDrawer::DrawMode)
                              (settings_->GetOption(OPT_TEXTURE_DRAW_MODE).toInt()));
  drawer_->SetUseTexture(settings_->GetOption(OPT_USE_TEXTURE).toBool());
  drawer_->SetString("88:88");

  active_plugins_ = settings_->GetOption(OPT_PLUGINS).toStringList();
  plugin_manager_->LoadPlugins(active_plugins_);

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

    case OPT_DISPLAY_AM_PM:
      d_clock_->SetDisplayAMPM(value.toBool());
      break;

    case OPT_USE_SKIN:
      if (value.toBool())
        skin_manager_->FindSkin(settings_->GetOption(OPT_SKIN_NAME).toString());
      break;

    case OPT_SKIN_NAME:
      if (settings_->GetOption(OPT_USE_SKIN).toBool())
        skin_manager_->FindSkin(value.toString());
      break;

    case OPT_USE_FONT:
      if (value.toBool())
        drawer_->LoadSkin(settings_->GetOption(OPT_FONT).value<QFont>());
      break;

    case OPT_FONT:
      if (settings_->GetOption(OPT_USE_FONT).toBool())
        drawer_->LoadSkin(value.value<QFont>());
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

    case OPT_USE_TEXTURE:
      drawer_->SetUseTexture(value.toBool());
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
  }
}

void MainWindow::ShowSettingsDialog() {
  // create settings dialog and connect all need signals
  // (settings dialog will be deleted automatically)
  SettingsDialog* settings_dlg = new SettingsDialog();
  connect(skin_manager_, SIGNAL(SearchFinished(QStringList)),
          settings_dlg, SLOT(SetSkinList(QStringList)));
  skin_manager_->ListSkins();
  connect(drawer_, SIGNAL(LoadedSkinInfo(TSkinInfo)),
          settings_dlg, SLOT(DisplaySkinInfo(TSkinInfo)));
  connect(plugin_manager_, SIGNAL(SearchFinished(QList<QPair<QString,bool> >)),
          settings_dlg, SLOT(SetPluginsList(QList<QPair<QString,bool> >)));
  connect(settings_dlg, SIGNAL(PluginInfoRequest(QString)),
          plugin_manager_, SLOT(GetPluginInfo(QString)));
  connect(plugin_manager_, SIGNAL(InfoGot(TPluginInfo)),
          settings_dlg, SLOT(DisplayPluginInfo(TPluginInfo)));
  plugin_manager_->ListAvailable();

  // reload settings to emit signals needed to init settings dialog controls
  // with current values
  connect(settings_, SIGNAL(OptionChanged(Options,QVariant)),
          settings_dlg, SLOT(SettingsListener(Options,QVariant)));
  settings_->TrackChanges(true);
  settings_->Load();
  settings_dlg->show();
  // disable settings listener for settings dialog
  disconnect(settings_, SIGNAL(OptionChanged(Options,QVariant)),
             settings_dlg, SLOT(SettingsListener(Options,QVariant)));
  // connect main logic signals: change/save/discard settings
  connect(settings_dlg, SIGNAL(OptionChanged(Options,QVariant)),
          settings_, SLOT(SetOption(Options,QVariant)));
  connect(settings_dlg, SIGNAL(PluginConfigureRequest(QString)),
          plugin_manager_, SLOT(ConfigurePlugin(QString)));
  connect(settings_dlg, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(settings_dlg, SIGNAL(rejected()), settings_, SLOT(Load()));

  connect(settings_dlg, SIGNAL(destroyed()), this, SLOT(EndSettingsEdit()));
  drawer_->SetPreviewMode(true);
}

void MainWindow::EndSettingsEdit() {
  drawer_->SetPreviewMode(false);
  settings_->TrackChanges(false);
}

void MainWindow::ShowAboutDialog() {
  AboutDialog* about_dlg = new AboutDialog();
  about_dlg->show();
}

void MainWindow::DisplayMenu(const QPoint& pos) {
  tray_control_->GetTrayIcon()->contextMenu()->exec(mapToParent(pos));
}

void MainWindow::ConnectAll() {
  connect(settings_, SIGNAL(OptionChanged(Options,QVariant)),
          this, SLOT(SettingsListener(Options,QVariant)));
  connect(skin_manager_, SIGNAL(SkinFound(QDir)), drawer_, SLOT(LoadSkin(QDir)));
  connect(drawer_, SIGNAL(DrawingFinished(QImage)), d_clock_, SLOT(DrawImage(QImage)));
  connect(d_clock_, SIGNAL(ImageNeeded(QString)), drawer_, SLOT(SetString(QString)));
  connect(tray_control_, SIGNAL(ShowSettingsDlg()), this, SLOT(ShowSettingsDialog()));
  connect(tray_control_, SIGNAL(ShowAboutDlg()), this, SLOT(ShowAboutDialog()));
  connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(DisplayMenu(QPoint)));
}

void MainWindow::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}
