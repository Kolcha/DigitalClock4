#include <QHBoxLayout>
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent) {
  d_clock_ = new DigitalClock();
  QHBoxLayout* main_layout = new QHBoxLayout();
  main_layout->addWidget(d_clock_);
  setLayout(main_layout);

  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);

  ConnectAll();
  //settings_.Load();
  skin_manager_.AddSkinDir(QDir(QDir::currentPath() + "/skins"));
  skin_manager_.ListSkins();
  skin_manager_.FindSkin("Comic Sans");
  drawer_.SetColor(Qt::blue);
  drawer_.SetZoom(1.25);
  drawer_.SetString("88:88");
}

void MainWindow::SetStaysOnTop(bool set) {
  SetWindowFlag(Qt::WindowStaysOnTopHint, set);
}

void MainWindow::SetTransparentForInput(bool set) {
  SetWindowFlag(Qt::WindowTransparentForInput, set);
}

void MainWindow::SettingsListener(Options opt, const QVariant& value) {
}

void MainWindow::ConnectAll() {
  connect(&settings_, SIGNAL(OptionChanged(Options,QVariant)),
          this, SLOT(SettingsListener(Options,QVariant)));
  connect(&skin_manager_, SIGNAL(SkinFound(QDir)), &drawer_, SLOT(LoadSkin(QDir)));
  connect(&drawer_, SIGNAL(DrawingFinished(QImage)), d_clock_, SLOT(DrawImage(QImage)));
  connect(d_clock_, SIGNAL(ImageNeeded(QString)), &drawer_, SLOT(SetString(QString)));
}

void MainWindow::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  hide();
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}
