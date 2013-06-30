#include <QHBoxLayout>
#include <QMouseEvent>
#include <QCoreApplication>
#include "main_window.h"

MainWindow::MainWindow(QWidget* parent)
  : QWidget(parent) {
  d_clock_ = new DigitalClock(this);
  QHBoxLayout* main_layout = new QHBoxLayout(this);
  main_layout->addWidget(d_clock_);
  setLayout(main_layout);

  setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
  setAttribute(Qt::WA_TranslucentBackground);

  tray_control_ = new TrayControl(this);

  settings_ = new ClockSettings(this);
  skin_manager_ = new SkinManager(this);
  drawer_ = new SkinDrawer(this);

  ConnectAll();
  skin_manager_->AddSkinDir(QDir(":/default_skin"));
  skin_manager_->AddSkinDir(QDir(QCoreApplication::applicationDirPath() + "/skins"));
  skin_manager_->ListSkins();
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
    settings_->SetOption(OPT_POSITION, pos());
    settings_->Save();
  }
}

void MainWindow::showEvent(QShowEvent* event) {
  on_show_ = true;
  if (!edit_settings_) settings_->Load();
  event->accept();
  on_show_ = false;
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

    case OPT_POSITION:
      move(value.toPoint());
      break;

    case OPT_SKIN_NAME:
      skin_manager_->FindSkin(value.toString());
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
  }
}

void MainWindow::ShowSettingsDialog() {
  // create settings dialog and connect all need signals
  // (settings dialog will be deleted automatically)
  settings_dlg_ = new SettingsDialog();
  edit_settings_ = true;
  connect(skin_manager_, SIGNAL(SearchFinished(QStringList)),
          settings_dlg_, SLOT(SetSkinList(QStringList)));
  skin_manager_->ListSkins();
  connect(drawer_, SIGNAL(LoadedSkinInfo(TSkinInfo)),
          settings_dlg_, SLOT(DisplaySkinInfo(TSkinInfo)));

  // reload settings to emit signals needed to init settings dialog controls
  // with current values
  connect(settings_, SIGNAL(OptionChanged(Options,QVariant)),
          settings_dlg_, SLOT(SettingsListener(Options,QVariant)));
  settings_->Load();
  settings_dlg_->show();
  // disable settings listener for settings dialog
  disconnect(settings_, SIGNAL(OptionChanged(Options,QVariant)),
             settings_dlg_, SLOT(SettingsListener(Options,QVariant)));
  // connect main logic signals: change/save/discard settings
  connect(settings_dlg_, SIGNAL(OptionChanged(Options,QVariant)),
          settings_, SLOT(SetOption(Options,QVariant)));
  connect(settings_dlg_, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(settings_dlg_, SIGNAL(rejected()), settings_, SLOT(Load()));

  connect(settings_dlg_, SIGNAL(destroyed()), this, SLOT(DisablePreviewMode()));
  drawer_->SetPreviewMode(true);
}

void MainWindow::DisablePreviewMode() {
  edit_settings_ = false;
  drawer_->SetPreviewMode(false);
}

void MainWindow::ConnectAll() {
  connect(settings_, SIGNAL(OptionChanged(Options,QVariant)),
          this, SLOT(SettingsListener(Options,QVariant)));
  connect(skin_manager_, SIGNAL(SkinFound(QDir)), drawer_, SLOT(LoadSkin(QDir)));
  connect(drawer_, SIGNAL(DrawingFinished(QImage)), d_clock_, SLOT(DrawImage(QImage)));
  connect(d_clock_, SIGNAL(ImageNeeded(QString)), drawer_, SLOT(SetString(QString)));
  connect(tray_control_, SIGNAL(ShowSettingsDlg()), this, SLOT(ShowSettingsDialog()));
}

void MainWindow::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  if (on_show_) return;
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}
